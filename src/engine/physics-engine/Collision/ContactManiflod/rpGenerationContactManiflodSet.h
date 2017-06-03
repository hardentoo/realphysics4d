/*
 * rpGenerationContactManiflodSet.h
 *
 *  Created on: 29 нояб. 2016 г.
 *      Author: wera
 */

#ifndef SOURCE_ENGIE_COLLISION_CONTACTMANIFLOD_RPGENERATIONCONTACTMANIFLODSET_H_
#define SOURCE_ENGIE_COLLISION_CONTACTMANIFLOD_RPGENERATIONCONTACTMANIFLODSET_H_


#include "../../Geometry/geometry.h"
#include "../rpProxyShape.h"
#include "rpContactManifoldSet.h"
#include "../../config.h"


namespace real_physics
{

class ContactManifoldSet;

class rpGenerationContactManiflodSet
{

	private:


		enum
		{
			eMaxContacts = 32
		};

	// -------------------- Attributes -------------------- //

		Transform trans1;
		Transform trans2;

		Vector3   mSeparatonAxis;

		const rpProxyShape *mShape1;
		const rpProxyShape *mShape2;


		rpContactPointInfo  mInfoContacts[eMaxContacts];
		uint                mNbContacts;


		/// Add contact point for collision pairs
		void addInfoContact(rpContactPointInfo info)
		{
			mInfoContacts[mNbContacts++] = info;
		}


        void CollidePointPointContacts(const Vector3& A , const Vector3& B );
        void CollidePointFaceContacts(const Vector3& A , const Vector3& xAxis, scalar bd );
        void CollidePointEdgeContacts( const Vector3& A , const Vector3& B0, const Vector3& B1 );
        void CollideEdgeEdgeContacts(const Vector3& A0, const Vector3& A1, const Vector3& B0, const  Vector3& B1 );
        void CollidePolygonContacts(const Vector3* Clipper, int iClipperSize, const Vector3* Poly, int iPolySize );



        bool ConvertSupportPointsToContacts(const Vector3* SupportVertA, int iNumVertsA,
                                            const Vector3* SupportVertB, int iNumVertsB );




   public:

	         rpGenerationContactManiflodSet(const rpProxyShape* shape1 , const rpProxyShape* shape2 , const Vector3& Axis);
	virtual ~rpGenerationContactManiflodSet();



         void computeContacteManiflodSet( rpContactManifoldSet& maniflodSet , bool approximationCorretion = INTERPOLATION_CONTACT_POINTS );



};




} /* namespace real_physics */

#endif /* SOURCE_ENGIE_COLLISION_CONTACTMANIFLOD_RPGENERATIONCONTACTMANIFLODSET_H_ */
