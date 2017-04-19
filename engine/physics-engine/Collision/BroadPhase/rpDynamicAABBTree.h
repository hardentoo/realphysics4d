/*
 * rpDynamicAABBTree.h
 *
 *  Created on: 23 нояб. 2016 г.
 *      Author: wera
 */

#ifndef SOURCE_ENGIE_COLLISION_BROADPHASE_RPDYNAMICAABBTREE_H_
#define SOURCE_ENGIE_COLLISION_BROADPHASE_RPDYNAMICAABBTREE_H_


#include "../Shapes/rpAABB.h"

namespace real_physics
{

// Declarations
class rpBroadPhaseAlgorithm;
class rpBroadPhaseRaycastTestCallback;
class rpDynamicAABBTreeOverlapCallback;

struct RaycastTest;

// Structure TreeNode
/**
 * This structure represents a node of the dynamic AABB tree.
 */
struct rpTreeNode
{

    // -------------------- Constants -------------------- //

    /// Null tree node constant
    const static int NULL_TREE_NODE;

    // -------------------- Attributes -------------------- //

    // A node is either in the tree (has a parent) or in the free nodes list
    // (has a next node)
    union
	{

        /// Parent node ID
        int32 parentID;

        /// Next allocated node ID
        int32 nextNodeID;
    };

    // A node is either a leaf (has data) or is an internal node (has children)
    union
	{

        /// Left and right child of the node (children[0] = left child)
        int32 children[2];

        /// Two pieces of data stored at that node (in case the node is a leaf)
        union
		{
            int32 dataInt[2];
            void* dataPointer;
        };
    };

    /// Height of the node in the tree
    int16 height;

    /// Fat axis aligned bounding box (AABB) corresponding to the node
    rpAABB aabb;

    // -------------------- Methods -------------------- //

    /// Return true if the node is a leaf of the tree
    bool isLeaf() const;
};




// Class DynamicAABBTreeOverlapCallback
/**
 * Overlapping callback method that has to be used as parameter of the
 * reportAllShapesOverlappingWithNode() method.
 */
class rpDynamicAABBTreeOverlapCallback
{

    public :

        // Called when a overlapping node has been found during the call to
        // DynamicAABBTree:reportAllShapesOverlappingWithAABB()
        virtual void notifyOverlappingNode(int nodeId)=0;
};




// Class DynamicAABBTreeRaycastCallback
/**
 * Raycast callback in the Dynamic AABB Tree called when the AABB of a leaf
 * node is hit by the ray.
 */
class rpDynamicAABBTreeRaycastCallback
{

    public:

        // Called when the AABB of a leaf node is hit by a ray
        virtual scalar raycastBroadPhaseShape(int32 nodeId, const Ray& ray)=0;

};

// Class DynamicAABBTree
/**
 * This class implements a dynamic AABB tree that is used for broad-phase
 * collision detection. This data structure is inspired by Nathanael Presson's
 * dynamic tree implementation in BulletPhysics. The following implementation is
 * based on the one from Erin Catto in Box2D as described in the book
 * "Introduction to Game Physics with Box2D" by Ian Parberry.
 */
class rpDynamicAABBTree
{

    private:

        // -------------------- Attributes -------------------- //

        /// Pointer to the memory location of the nodes of the tree
	    rpTreeNode* mNodes;

        /// ID of the root node of the tree
        int mRootNodeID;

        /// ID of the first node of the list of free (allocated) nodes in the tree that we can use
        int mFreeNodeID;

        /// Number of allocated nodes in the tree
        int mNbAllocatedNodes;

        /// Number of nodes in the tree
        int mNbNodes;

        /// Extra AABB Gap used to allow the collision shape to move a little bit
        /// without triggering a large modification of the tree which can be costly
        scalar mExtraAABBGap;

        // -------------------- Methods -------------------- //

        /// Allocate and return a node to use in the tree
        int allocateNode();

        /// Release a node
        void releaseNode(int nodeID);

        /// Insert a leaf node in the tree
        void insertLeafNode(int nodeID);

        /// Remove a leaf node from the tree
        void removeLeafNode(int nodeID);

        /// Balance the sub-tree of a given node using left or right rotations.
        int balanceSubTreeAtNode(int nodeID);

        /// Compute the height of a given node in the tree
        int computeHeight(int nodeID);

        /// Internally add an object into the tree
        int addObjectInternal(const rpAABB& aabb);

        /// Initialize the tree
        void init();

#ifndef NDEBUG

        /// Check if the tree structure is valid (for debugging purpose)
        void check() const;

        /// Check if the node structure is valid (for debugging purpose)
        void checkNode(int nodeID) const;

#endif

    public:

        // -------------------- Methods -------------------- //

        /// Constructor
        rpDynamicAABBTree(scalar extraAABBGap = scalar(0.0));

        /// Destructor
        ~rpDynamicAABBTree();

        /// Add an object into the tree (where node data are two integers)
        int addObject(const rpAABB& aabb, int32 data1, int32 data2);

        /// Add an object into the tree (where node data is a pointer)
        int addObject(const rpAABB& aabb, void* data);

        /// Remove an object from the tree
        void removeObject(int nodeID);

        /// Update the dynamic tree after an object has moved.
        bool updateObject(int nodeID, const rpAABB& newAABB, const Vector3& displacement, bool forceReinsert = false);

        /// Return the fat AABB corresponding to a given node ID
        const rpAABB& getFatAABB(int nodeID) const;

        /// Return the pointer to the data array of a given leaf node of the tree
        int32* getNodeDataInt(int nodeID) const;

        /// Return the data pointer of a given leaf node of the tree
        void* getNodeDataPointer(int nodeID) const;

        /// Report all shapes overlapping with the AABB given in parameter.
        void reportAllShapesOverlappingWithAABB(const rpAABB& aabb,
                                                rpDynamicAABBTreeOverlapCallback& callback) const;

        /// Ray casting method
        void raycast(const Ray& ray, rpDynamicAABBTreeRaycastCallback& callback) const;

        /// Compute the height of the tree
        int computeHeight();

        /// Return the root AABB of the tree
        rpAABB getRootAABB() const;

        /// Clear all the nodes and reset the tree
        void reset();
};

// Return true if the node is a leaf of the tree
SIMD_INLINE bool rpTreeNode::isLeaf() const
{
    return (height == 0);
}

// Return the fat AABB corresponding to a given node ID
SIMD_INLINE const rpAABB& rpDynamicAABBTree::getFatAABB(int nodeID) const
{
    assert(nodeID >= 0 && nodeID < mNbAllocatedNodes);
    return mNodes[nodeID].aabb;
}

// Return the pointer to the data array of a given leaf node of the tree
SIMD_INLINE int32* rpDynamicAABBTree::getNodeDataInt(int nodeID) const
{
    assert(nodeID >= 0 && nodeID < mNbAllocatedNodes);
    assert(mNodes[nodeID].isLeaf());
    return mNodes[nodeID].dataInt;
}

// Return the pointer to the data pointer of a given leaf node of the tree
SIMD_INLINE void* rpDynamicAABBTree::getNodeDataPointer(int nodeID) const
{
    assert(nodeID >= 0 && nodeID < mNbAllocatedNodes);
    assert(mNodes[nodeID].isLeaf());
    return mNodes[nodeID].dataPointer;
}

// Return the root AABB of the tree
SIMD_INLINE rpAABB rpDynamicAABBTree::getRootAABB() const
{
    return getFatAABB(mRootNodeID);
}

// Add an object into the tree. This method creates a new leaf node in the tree and
// returns the ID of the corresponding node.
SIMD_INLINE int rpDynamicAABBTree::addObject(const rpAABB& aabb, int32 data1, int32 data2)
{

    int nodeId = addObjectInternal(aabb);

    mNodes[nodeId].dataInt[0] = data1;
    mNodes[nodeId].dataInt[1] = data2;

    return nodeId;
}

// Add an object into the tree. This method creates a new leaf node in the tree and
// returns the ID of the corresponding node.
SIMD_INLINE int rpDynamicAABBTree::addObject(const rpAABB& aabb, void* data)
{

    int nodeId = addObjectInternal(aabb);

    mNodes[nodeId].dataPointer = data;

    return nodeId;
}




} /* namespace real_physics */

#endif /* SOURCE_ENGIE_COLLISION_BROADPHASE_RPDYNAMICAABBTREE_H_ */
