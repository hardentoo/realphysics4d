/*
 * rpSliderJoint.h
 *
 *  Created on: 10 янв. 2017 г.
 *      Author: wera
 */

#ifndef SOURCE_ENGIE_KINEMATICPHYSICS_CONSTRAINT_RPSLIDERJOINT_H_
#define SOURCE_ENGIE_KINEMATICPHYSICS_CONSTRAINT_RPSLIDERJOINT_H_

#include "rpJoint.h"

namespace real_physics
{



// Structure SliderJointInfo
/**
 * This structure is used to gather the information needed to create a slider
 * joint. This structure will be used to create the actual slider joint.
 */
struct rpSliderJointInfo : public rpJointInfo
{

    public :

        // -------------------- Attributes -------------------- //

        /// Anchor point (in world-space coordinates)
        Vector3 anchorPointWorldSpace;

        /// Slider axis (in world-space coordinates)
        Vector3 sliderAxisWorldSpace;

        /// True if the slider limits are enabled
        bool isLimitEnabled;

        /// True if the slider motor is enabled
        bool isMotorEnabled;

        /// Mininum allowed translation if limits are enabled
        scalar minTranslationLimit;

        /// Maximum allowed translation if limits are enabled
        scalar maxTranslationLimit;

        /// Motor speed
        scalar motorSpeed;

        /// Maximum motor force (in Newtons) that can be applied to reach to desired motor speed
        scalar maxMotorForce;

        /// Constructor without limits and without motor
        /**
         * @param rigidBody1 The first body of the joint
         * @param rigidBody2 The second body of the joint
         * @param initAnchorPointWorldSpace The initial anchor point in world-space
         * @param initSliderAxisWorldSpace The initial slider axis in world-space
         */
        rpSliderJointInfo(rpPhysicsBody* rigidBody1, rpPhysicsBody* rigidBody2,
                         const Vector3& initAnchorPointWorldSpace,
                         const Vector3& initSliderAxisWorldSpace)
                       :rpJointInfo(rigidBody1, rigidBody2, SLIDERJOINT),
                         anchorPointWorldSpace(initAnchorPointWorldSpace),
                         sliderAxisWorldSpace(initSliderAxisWorldSpace),
                         isLimitEnabled(false), isMotorEnabled(false), minTranslationLimit(-1.0),
                         maxTranslationLimit(1.0), motorSpeed(0), maxMotorForce(0)
        {

        }

        /// Constructor with limits and no motor
        /**
         * @param rigidBody1 The first body of the joint
         * @param rigidBody2 The second body of the joint
         * @param initAnchorPointWorldSpace The initial anchor point in world-space
         * @param initSliderAxisWorldSpace The initial slider axis in world-space
         * @param initMinTranslationLimit The initial minimum translation limit (in meters)
         * @param initMaxTranslationLimit The initial maximum translation limit (in meters)
         */
        rpSliderJointInfo(rpPhysicsBody* rigidBody1, rpPhysicsBody* rigidBody2,
                          const Vector3& initAnchorPointWorldSpace,
                          const Vector3& initSliderAxisWorldSpace,
                          scalar initMinTranslationLimit, scalar initMaxTranslationLimit)
                          :rpJointInfo(rigidBody1, rigidBody2, SLIDERJOINT),
                         anchorPointWorldSpace(initAnchorPointWorldSpace),
                         sliderAxisWorldSpace(initSliderAxisWorldSpace),
                         isLimitEnabled(true), isMotorEnabled(false),
                         minTranslationLimit(initMinTranslationLimit),
                         maxTranslationLimit(initMaxTranslationLimit), motorSpeed(0),
                         maxMotorForce(0)
        {

        }

        /// Constructor with limits and motor
        /**
         * @param rigidBody1 The first body of the joint
         * @param rigidBody2 The second body of the joint
         * @param initAnchorPointWorldSpace The initial anchor point in world-space
         * @param initSliderAxisWorldSpace The initial slider axis in world-space
         * @param initMinTranslationLimit The initial minimum translation limit (in meters)
         * @param initMaxTranslationLimit The initial maximum translation limit (in meters)
         * @param initMotorSpeed The initial speed of the joint motor (in meters per second)
         * @param initMaxMotorForce The initial maximum motor force of the joint (in Newtons x meters)
         */
        rpSliderJointInfo(rpPhysicsBody* rigidBody1, rpPhysicsBody* rigidBody2,
                          const Vector3& initAnchorPointWorldSpace,
                          const Vector3& initSliderAxisWorldSpace,
                          scalar initMinTranslationLimit, scalar initMaxTranslationLimit,
                          scalar initMotorSpeed, scalar initMaxMotorForce)
                         :rpJointInfo(rigidBody1, rigidBody2, SLIDERJOINT),
                         anchorPointWorldSpace(initAnchorPointWorldSpace),
                         sliderAxisWorldSpace(initSliderAxisWorldSpace),
                         isLimitEnabled(true), isMotorEnabled(true),
                         minTranslationLimit(initMinTranslationLimit),
                         maxTranslationLimit(initMaxTranslationLimit), motorSpeed(initMotorSpeed),
                         maxMotorForce(initMaxMotorForce)
        {

        }
};



// Class SliderJoint
/**
 * This class represents a slider joint. This joint has a one degree of freedom.
 * It only allows relative translation of the bodies along a single direction and no
 * rotation.
 */
class rpSliderJoint : public rpJoint
{

    private :


		bool isWarmStartingActive = true;

		rpRigidPhysicsBody *Body1;
		rpRigidPhysicsBody *Body2;
        // -------------------- Constants -------------------- //

        // Beta value for the position correction bias factor
        static const scalar BETA;

        // -------------------- Attributes -------------------- //

        /// Anchor point of body 1 (in local-space coordinates of body 1)
        Vector3 mLocalAnchorPointBody1;

        /// Anchor point of body 2 (in local-space coordinates of body 2)
        Vector3 mLocalAnchorPointBody2;

        /// Slider axis (in local-space coordinates of body 1)
        Vector3 mSliderAxisBody1;

        /// Inertia tensor of body 1 (in world-space coordinates)
        Matrix3x3 mI1;

        /// Inertia tensor of body 2 (in world-space coordinates)
        Matrix3x3 mI2;

        /// Inverse of the initial orientation difference between the two bodies
        Quaternion mInitOrientationDifferenceInv;

        /// First vector orthogonal to the slider axis local-space of body 1
        Vector3 mN1;

        /// Second vector orthogonal to the slider axis and mN1 in local-space of body 1
        Vector3 mN2;

        /// Vector r1 in world-space coordinates
        Vector3 mR1;

        /// Vector r2 in world-space coordinates
        Vector3 mR2;

        /// Cross product of r2 and n1
        Vector3 mR2CrossN1;

        /// Cross product of r2 and n2
        Vector3 mR2CrossN2;

        /// Cross product of r2 and the slider axis
        Vector3 mR2CrossSliderAxis;

        /// Cross product of vector (r1 + u) and n1
        Vector3 mR1PlusUCrossN1;

        /// Cross product of vector (r1 + u) and n2
        Vector3 mR1PlusUCrossN2;

        /// Cross product of vector (r1 + u) and the slider axis
        Vector3 mR1PlusUCrossSliderAxis;

        /// Bias of the 2 translation constraints
        Vector2 mBTranslation;

        /// Bias of the 3 rotation constraints
        Vector3 mBRotation;

        /// Bias of the lower limit constraint
        scalar mBLowerLimit;

        /// Bias of the upper limit constraint
        scalar mBUpperLimit;

        /// Inverse of mass matrix K=JM^-1J^t for the translation constraint (2x2 matrix)
        Matrix2x2 mInverseMassMatrixTranslationConstraint;

        /// Inverse of mass matrix K=JM^-1J^t for the rotation constraint (3x3 matrix)
        Matrix3x3 mInverseMassMatrixRotationConstraint;

        /// Inverse of mass matrix K=JM^-1J^t for the upper and lower limit constraints (1x1 matrix)
        scalar mInverseMassMatrixLimit;

        /// Inverse of mass matrix K=JM^-1J^t for the motor
        scalar mInverseMassMatrixMotor;

        /// Accumulated impulse for the 2 translation constraints
        Vector2 mImpulseTranslation;

        /// Accumulated impulse for the 3 rotation constraints
        Vector3 mImpulseRotation;

        /// Accumulated impulse for the lower limit constraint
        scalar mImpulseLowerLimit;

        /// Accumulated impulse for the upper limit constraint
        scalar mImpulseUpperLimit;

        /// Accumulated impulse for the motor
        scalar mImpulseMotor;

        /// True if the slider limits are enabled
        bool mIsLimitEnabled;

        /// True if the motor of the joint in enabled
        bool mIsMotorEnabled;

        /// Slider axis in world-space coordinates
        Vector3 mSliderAxisWorld;

        /// Lower limit (minimum translation distance)
        scalar mLowerLimit;

        /// Upper limit (maximum translation distance)
        scalar mUpperLimit;

        /// True if the lower limit is violated
        bool mIsLowerLimitViolated;

        /// True if the upper limit is violated
        bool mIsUpperLimitViolated;

        /// Motor speed (in m/s)
        scalar mMotorSpeed;

        /// Maximum motor force (in Newtons) that can be applied to reach to desired motor speed
        scalar mMaxMotorForce;

        // -------------------- Methods -------------------- //

        /// Private copy-constructor
        rpSliderJoint(const rpSliderJoint& constraint);

        /// Private assignment operator
        rpSliderJoint& operator=(const rpSliderJoint& constraint);

        /// Reset the limits
        void resetLimits();

        /// Return the number of bytes used by the joint
        virtual size_t getSizeInBytes() const;

        /// Initialize before solving the constraint
        virtual void initBeforeSolve( scalar timeStep );

        /// Warm start the constraint (apply the previous impulse at the beginning of the step)
        virtual void warmstart();

        /// Solve the velocity constraint
        virtual void solveVelocityConstraint();

        /// Solve the position constraint (for position error correction)
        virtual void solvePositionConstraint();

    public :

        // -------------------- Methods -------------------- //

        /// Constructor
        rpSliderJoint(const rpSliderJointInfo& jointInfo);

        /// Destructor
        virtual ~rpSliderJoint();

        /// Return true if the limits or the joint are enabled
        bool isLimitEnabled() const;

        /// Return true if the motor of the joint is enabled
        bool isMotorEnabled() const;

        /// Enable/Disable the limits of the joint
        void enableLimit(bool isLimitEnabled);

        /// Enable/Disable the motor of the joint
        void enableMotor(bool isMotorEnabled);

        /// Return the current translation value of the joint
        scalar getTranslation() const;

        /// Return the minimum translation limit
        scalar getMinTranslationLimit() const;

        /// Set the minimum translation limit
        void setMinTranslationLimit(scalar lowerLimit);

        /// Return the maximum translation limit
        scalar getMaxTranslationLimit() const;

        /// Set the maximum translation limit
        void setMaxTranslationLimit(scalar upperLimit);

        /// Return the motor speed
        scalar getMotorSpeed() const;

        /// Set the motor speed
        void setMotorSpeed(scalar motorSpeed);

        /// Return the maximum motor force
        scalar getMaxMotorForce() const;

        /// Set the maximum motor force
        void setMaxMotorForce(scalar maxMotorForce);

        /// Return the intensity of the current force applied for the joint motor
        scalar getMotorForce(scalar timeStep) const;
};

// Return true if the limits or the joint are enabled
/**
 * @return True if the joint limits are enabled
 */
SIMD_INLINE bool rpSliderJoint::isLimitEnabled() const
{
    return mIsLimitEnabled;
}

// Return true if the motor of the joint is enabled
/**
 * @return True if the joint motor is enabled
 */
SIMD_INLINE bool rpSliderJoint::isMotorEnabled() const
{
    return mIsMotorEnabled;
}

// Return the minimum translation limit
/**
 * @return The minimum translation limit of the joint (in meters)
 */
SIMD_INLINE scalar rpSliderJoint::getMinTranslationLimit() const
{
    return mLowerLimit;
}

// Return the maximum translation limit
/**
 * @return The maximum translation limit of the joint (in meters)
 */
SIMD_INLINE scalar rpSliderJoint::getMaxTranslationLimit() const
{
    return mUpperLimit;
}

// Return the motor speed
/**
 * @return The current motor speed of the joint (in meters per second)
 */
SIMD_INLINE scalar rpSliderJoint::getMotorSpeed() const
{
    return mMotorSpeed;
}

// Return the maximum motor force
/**
 * @return The maximum force of the joint motor (in Newton x meters)
 */
SIMD_INLINE scalar rpSliderJoint::getMaxMotorForce() const
{
    return mMaxMotorForce;
}

// Return the intensity of the current force applied for the joint motor
/**
 * @param timeStep Time step (in seconds)
 * @return The current force of the joint motor (in Newton x meters)
 */
SIMD_INLINE scalar rpSliderJoint::getMotorForce(scalar timeStep) const
{
    return mImpulseMotor / timeStep;
}

// Return the number of bytes used by the joint
SIMD_INLINE size_t rpSliderJoint::getSizeInBytes() const
{
    return sizeof(rpSliderJoint);
}


} /* namespace real_physics */

#endif /* SOURCE_ENGIE_KINEMATICPHYSICS_CONSTRAINT_RPSLIDERJOINT_H_ */
