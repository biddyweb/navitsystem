/* This is a template file generated by command */
/* orbit-idl-2 --skeleton-impl map.idl */
/* User must edit this file, inserting servant  */
/* specific code between markers. */

#include "map.h"

/*** App-specific servant structures ***/

typedef struct
{
   POA_Mappel servant;
   PortableServer_POA poa;

   /* ------ add private attributes here ------ */
   /* ------ ---------- end ------------ ------ */
} impl_POA_Mappel;

typedef struct
{
   POA_Map servant;
   PortableServer_POA poa;

   /* ------ add private attributes here ------ */
   /* ------ ---------- end ------------ ------ */
} impl_POA_Map;

/*** Implementation stub prototypes ***/

static void impl_Mappel__destroy(impl_POA_Mappel * servant,
				 CORBA_Environment * ev);
static void
impl_Mappel_Test(impl_POA_Mappel * servant, CORBA_Environment * ev);

static void impl_Map__destroy(impl_POA_Map * servant, CORBA_Environment * ev);
static Map
impl_Map_mapString(impl_POA_Map * servant,
		   const CORBA_char * astring,
		   CORBA_double * anum, CORBA_Environment * ev);

static void
impl_Map_doNothing(impl_POA_Map * servant, CORBA_Environment * ev);

static void
impl_Map_doOneWay(impl_POA_Map * servant,
		  const CORBA_char * ignore, CORBA_Environment * ev);

static void
impl_Map_PlaceFlag(impl_POA_Map * servant, CORBA_Environment * ev);

static PointObj
impl_Map_PointFromCoord(impl_POA_Map * servant,
			const CORBA_char * coord, CORBA_Environment * ev);

static void
impl_Map_View(impl_POA_Map * servant,
	      const PointObj * where, CORBA_Environment * ev);

static void
impl_Map_ViewAll(impl_POA_Map * servant,
		 const PointObjSequence * where, CORBA_Environment * ev);

static void
impl_Map_Route(impl_POA_Map * servant,
	       const PointObj * src,
	       const PointObj * dst, CORBA_Environment * ev);

static Mappel impl_Map_Get(impl_POA_Map * servant, CORBA_Environment * ev);

/*** epv structures ***/

static PortableServer_ServantBase__epv impl_Mappel_base_epv = {
   NULL,			/* _private data */
   (gpointer) & impl_Mappel__destroy,	/* finalize routine */
   NULL,			/* default_POA routine */
};
static POA_Mappel__epv impl_Mappel_epv = {
   NULL,			/* _private */
   (gpointer) & impl_Mappel_Test,

};
static PortableServer_ServantBase__epv impl_Map_base_epv = {
   NULL,			/* _private data */
   (gpointer) & impl_Map__destroy,	/* finalize routine */
   NULL,			/* default_POA routine */
};
static POA_Map__epv impl_Map_epv = {
   NULL,			/* _private */
   (gpointer) & impl_Map_mapString,

   (gpointer) & impl_Map_doNothing,

   (gpointer) & impl_Map_doOneWay,

   (gpointer) & impl_Map_PlaceFlag,

   (gpointer) & impl_Map_PointFromCoord,

   (gpointer) & impl_Map_View,

   (gpointer) & impl_Map_ViewAll,

   (gpointer) & impl_Map_Route,

   (gpointer) & impl_Map_Get,

};

/*** vepv structures ***/

static POA_Mappel__vepv impl_Mappel_vepv = {
   &impl_Mappel_base_epv,
   &impl_Mappel_epv,
};
static POA_Map__vepv impl_Map_vepv = {
   &impl_Map_base_epv,
   &impl_Map_epv,
};

/*** Stub implementations ***/

static Mappel
impl_Mappel__create(PortableServer_POA poa, CORBA_Environment * ev)
{
   Mappel retval;
   impl_POA_Mappel *newservant;
   PortableServer_ObjectId *objid;

   newservant = g_new0(impl_POA_Mappel, 1);
   newservant->servant.vepv = &impl_Mappel_vepv;
   newservant->poa =
      (PortableServer_POA) CORBA_Object_duplicate((CORBA_Object) poa, ev);
   POA_Mappel__init((PortableServer_Servant) newservant, ev);
   /* Before servant is going to be activated all
    * private attributes must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

   objid = PortableServer_POA_activate_object(poa, newservant, ev);
   CORBA_free(objid);
   retval = PortableServer_POA_servant_to_reference(poa, newservant, ev);

   return retval;
}

static void
impl_Mappel__destroy(impl_POA_Mappel * servant, CORBA_Environment * ev)
{
   CORBA_Object_release((CORBA_Object) servant->poa, ev);

   /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

   POA_Mappel__fini((PortableServer_Servant) servant, ev);
}

static void
impl_Mappel_Test(impl_POA_Mappel * servant, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static Map
impl_Map__create(PortableServer_POA poa, CORBA_Environment * ev)
{
   Map retval;
   impl_POA_Map *newservant;
   PortableServer_ObjectId *objid;

   newservant = g_new0(impl_POA_Map, 1);
   newservant->servant.vepv = &impl_Map_vepv;
   newservant->poa =
      (PortableServer_POA) CORBA_Object_duplicate((CORBA_Object) poa, ev);
   POA_Map__init((PortableServer_Servant) newservant, ev);
   /* Before servant is going to be activated all
    * private attributes must be initialized.  */

   /* ------ init private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

   objid = PortableServer_POA_activate_object(poa, newservant, ev);
   CORBA_free(objid);
   retval = PortableServer_POA_servant_to_reference(poa, newservant, ev);

   return retval;
}

static void
impl_Map__destroy(impl_POA_Map * servant, CORBA_Environment * ev)
{
   CORBA_Object_release((CORBA_Object) servant->poa, ev);

   /* No further remote method calls are delegated to 
    * servant and you may free your private attributes. */
   /* ------ free private attributes here ------ */
   /* ------ ---------- end ------------- ------ */

   POA_Map__fini((PortableServer_Servant) servant, ev);
}

static Map
impl_Map_mapString(impl_POA_Map * servant,
		   const CORBA_char * astring,
		   CORBA_double * anum, CORBA_Environment * ev)
{
   Map retval;

   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */

   return retval;
}

static void
impl_Map_doNothing(impl_POA_Map * servant, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static void
impl_Map_doOneWay(impl_POA_Map * servant,
		  const CORBA_char * ignore, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static void
impl_Map_PlaceFlag(impl_POA_Map * servant, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static PointObj
impl_Map_PointFromCoord(impl_POA_Map * servant,
			const CORBA_char * coord, CORBA_Environment * ev)
{
   PointObj retval;

   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */

   return retval;
}

static void
impl_Map_View(impl_POA_Map * servant,
	      const PointObj * where, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static void
impl_Map_ViewAll(impl_POA_Map * servant,
		 const PointObjSequence * where, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static void
impl_Map_Route(impl_POA_Map * servant,
	       const PointObj * src,
	       const PointObj * dst, CORBA_Environment * ev)
{
   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */
}

static Mappel
impl_Map_Get(impl_POA_Map * servant, CORBA_Environment * ev)
{
   Mappel retval;

   /* ------   insert method code here   ------ */
   /* ------ ---------- end ------------ ------ */

   return retval;
}
