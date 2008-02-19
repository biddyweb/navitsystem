#include <config.h>
#include <gps.h>
#include <string.h>
#include <glib.h>
#include <math.h>
#include "debug.h"
#include "callback.h"
#include "plugin.h"
#include "coord.h"
#include "item.h"
#include "vehicle.h"

static struct vehicle_priv {
	char *source;
	struct callback_list *cbl;
	GIOChannel *iochan;
	guint retry_interval;
	guint watch;
	struct gps_data_t *gps;
	struct coord_geo geo;
	double speed;
	double direction;
	double height;
	int status;
	int sats;
	int sats_used;
} *vehicle_last;

#define DEFAULT_RETRY_INTERVAL 10 // seconds
#define MIN_RETRY_INTERVAL 1 // seconds

static gboolean vehicle_gpsd_io(GIOChannel * iochan,
				GIOCondition condition, gpointer t);



static void
vehicle_gpsd_callback(struct gps_data_t *data, char *buf, size_t len,
		      int level)
{
	struct vehicle_priv *priv = vehicle_last;
	dbg(1,"data->set=0x%x\n", data->set);
	// If data->fix.speed is NAN, then the drawing gets jumpy. 
	if (isnan(data->fix.speed)) {
		return;
	}
	dbg(2,"speed ok\n");
	if (data->set & SPEED_SET) {
		priv->speed = data->fix.speed * 3.6;
		data->set &= ~SPEED_SET;
	}
	if (data->set & TRACK_SET) {
		priv->direction = data->fix.track;
		data->set &= ~TRACK_SET;
	}
	if (data->set & ALTITUDE_SET) {
		priv->height = data->fix.altitude;
		data->set &= ~ALTITUDE_SET;
	}
	if (data->set & SATELLITE_SET) {
		priv->sats_used = data->satellites_used;
		priv->sats = data->satellites;
		data->set &= ~SATELLITE_SET;
	}
	if (data->set & STATUS_SET) {
		priv->status = data->status;
		data->set &= ~STATUS_SET;
	}
	if (data->set & PDOP_SET) {
		dbg(0, "pdop : %g\n", data->pdop);
		data->set &= ~PDOP_SET;
	}
	if (data->set & LATLON_SET) {
		priv->geo.lat = data->fix.latitude;
		priv->geo.lng = data->fix.longitude;
		dbg(1,"lat=%f lng=%f\n", priv->geo.lat, priv->geo.lng);
		callback_list_call_0(priv->cbl);
		data->set &= ~LATLON_SET;
	}
}

/**
 * Attempt to open the gps device.
 * Return FALSE if retry not required
 * Return TRUE to try again
 */
static gboolean
vehicle_gpsd_try_open(gpointer *data)
{
	struct vehicle_priv *priv = (struct vehicle_priv *)data;
	char *source = g_strdup(priv->source);
	char *colon = index(source + 7, ':');
	if (colon) {
		*colon = '\0';
		priv->gps = gps_open(source + 7, colon + 1);
	} else
		priv->gps = gps_open(source + 7, NULL);
	g_free(source);

	if (!priv->gps){
		g_warning("gps_open failed for '%s'. Retrying in %d seconds. Have you started gpsd?\n", priv->source, priv->retry_interval);
		return TRUE;
	}
	gps_query(priv->gps, "w+x\n");
	gps_set_raw_hook(priv->gps, vehicle_gpsd_callback);
	priv->iochan = g_io_channel_unix_new(priv->gps->gps_fd);
	priv->watch =
	    g_io_add_watch(priv->iochan, G_IO_IN | G_IO_ERR | G_IO_HUP,
			   vehicle_gpsd_io, priv);
	dbg(0,"Connected to gpsd\n");
	return FALSE;
}

/**
 * Open a connection to gpsd. Will re-try the connection if it fails
 */
static void
vehicle_gpsd_open(struct vehicle_priv *priv)
{
	if (vehicle_gpsd_try_open((gpointer *)priv)) {
		g_timeout_add(priv->retry_interval*1000, (GSourceFunc)vehicle_gpsd_try_open, (gpointer *)priv);
	}
}

static void
vehicle_gpsd_close(struct vehicle_priv *priv)
{
	GError *error = NULL;

	if (priv->watch) {
		g_source_remove(priv->watch);
		priv->watch = 0;
	}
	if (priv->iochan) {
		g_io_channel_shutdown(priv->iochan, 0, &error);
		priv->iochan = NULL;
	}
	if (priv->gps) {
		gps_close(priv->gps);
		priv->gps = NULL;
	}
}

static gboolean
vehicle_gpsd_io(GIOChannel * iochan, GIOCondition condition, gpointer t)
{
	struct vehicle_priv *priv = t;

	dbg(1, "enter condition=%d\n", condition);
	if (condition == G_IO_IN) {
		if (priv->gps) {
			vehicle_last = priv;
                        if (gps_poll(priv->gps)) {
                               g_warning("gps_poll failed\n");
                               vehicle_gpsd_close(priv);
                               vehicle_gpsd_open(priv);
                        }
		}
		return TRUE;
	}
	return FALSE;
}

static void
vehicle_gpsd_destroy(struct vehicle_priv *priv)
{
	vehicle_gpsd_close(priv);
	if (priv->source)
		g_free(priv->source);
	g_free(priv);
}

static int
vehicle_gpsd_position_attr_get(struct vehicle_priv *priv,
			       enum attr_type type, struct attr *attr)
{
	switch (type) {
	case attr_position_height:
		attr->u.numd = &priv->height;
		break;
	case attr_position_speed:
		attr->u.numd = &priv->speed;
		break;
	case attr_position_direction:
		attr->u.numd = &priv->direction;
		break;
	case attr_position_sats:
		attr->u.num = priv->sats;
		break;
	case attr_position_sats_used:
		attr->u.num = priv->sats_used;
		break;
	case attr_position_coord_geo:
		attr->u.coord_geo = &priv->geo;
		break;
	default:
		return 0;
	}
	attr->type = type;
	return 1;
}

struct vehicle_methods vehicle_gpsd_methods = {
	vehicle_gpsd_destroy,
	vehicle_gpsd_position_attr_get,
};

static struct vehicle_priv *
vehicle_gpsd_new_gpsd(struct vehicle_methods
		      *meth, struct callback_list
		      *cbl, struct attr **attrs)
{
	struct vehicle_priv *ret;
	struct attr *source, *retry_int;

	dbg(1, "enter\n");
	source = attr_search(attrs, NULL, attr_source);
	ret = g_new0(struct vehicle_priv, 1);
	ret->source = g_strdup(source->u.str);
	retry_int = attr_search(attrs, NULL, attr_retry_interval);
	if (retry_int) {
		ret->retry_interval = retry_int->u.num;
		if (ret->retry_interval < MIN_RETRY_INTERVAL) {
			dbg(0, "Retry interval %d too small, setting to %d\n", ret->retry_interval, MIN_RETRY_INTERVAL);
			ret->retry_interval = MIN_RETRY_INTERVAL;
		}
	} else {
		dbg(0, "Retry interval not defined, setting to %d\n", DEFAULT_RETRY_INTERVAL);
		ret->retry_interval = DEFAULT_RETRY_INTERVAL;
	}
	ret->cbl = cbl;
	*meth = vehicle_gpsd_methods;
	vehicle_gpsd_open(ret);
	return ret;
}

void
plugin_init(void)
{
	dbg(1, "enter\n");
	plugin_register_vehicle_type("gpsd", vehicle_gpsd_new_gpsd);
}
