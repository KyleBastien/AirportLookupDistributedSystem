/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "places.h"

bool_t
xdr_placeslist (XDR *xdrs, placeslist *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct placesnode), (xdrproc_t) xdr_placesnode))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_placesnode (XDR *xdrs, placesnode *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->code, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->name, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->state, 2))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_placeslist (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_location (XDR *xdrs, location *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->city, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->state, 2))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->host, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readplaces_ret (XDR *xdrs, readplaces_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->err))
		 return FALSE;
	switch (objp->err) {
	case 0:
		 if (!xdr_placeslist (xdrs, &objp->readplaces_ret_u.list))
			 return FALSE;
		break;
	case 1:
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
