//
// Copyright (c) 2015-2020 CNRS INRIA
//

#ifndef __pinocchio_algorithm_frames_hpp__
#define __pinocchio_algorithm_frames_hpp__

#include "pinocchio/multibody/model.hpp"
#include "pinocchio/multibody/data.hpp"

namespace pinocchio
{

  /**
   * @brief      Updates the position of each frame contained in the model.
   *
   * @tparam JointCollection Collection of Joint types.
   *
   * @param[in]  model  The kinematic model.
   * @param      data   Data associated to model.
   *
   * @warning    One of the algorithms forwardKinematics should have been called first.
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline void updateFramePlacements(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data);

  /**
   * @brief      Updates the placement of the given frame.
   *
   * @param[in]  model        The kinematic model.
   * @param      data         Data associated to model.
   * @param[in]  frame_id     Id of the operational Frame.
   *
   * @return     A reference to the frame placement stored in data.oMf[frame_id]
   *
   * @warning    One of the algorithms forwardKinematics should have been called first
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline const typename DataTpl<Scalar, Options, JointCollectionTpl>::SE3 & updateFramePlacement(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id);

  /**
   * @brief      First calls the forwardKinematics on the model, then computes the placement of each
   * frame. /sa pinocchio::forwardKinematics.
   *
   * @tparam JointCollection Collection of Joint types.
   * @tparam ConfigVectorType Type of the joint configuration vector.
   *
   * @param[in]  model                    The kinematic model.
   * @param      data                     Data associated to model.
   * @param[in]  q                        Configuration vector.
   */
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename ConfigVectorType>
  inline void framesForwardKinematics(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const Eigen::MatrixBase<ConfigVectorType> & q);

  /**
   * @brief      Returns the spatial velocity of the Frame expressed in the desired reference frame.
   *             You must first call pinocchio::forwardKinematics to update placement and velocity
   * values in data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  joint_id    Id of the parent joint
   * @param[in]  placement   frame placement with respect to the parent joint
   * @param[in]  rf          Reference frame in which the velocity is expressed.
   *
   * @return     The spatial velocity of the Frame expressed in the desired reference frame.
   *
   * @warning    Fist or second order forwardKinematics should have been called first
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameVelocity(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const JointIndex joint_id,
    const SE3Tpl<Scalar, Options> & placement,
    const ReferenceFrame rf = LOCAL);

  /**
   * @brief      Returns the spatial velocity of the Frame expressed in the desired reference frame.
   *             You must first call pinocchio::forwardKinematics to update placement and velocity
   * values in data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  frame_id    Id of the operational Frame
   * @param[in]  rf          Reference frame in which the velocity is expressed.
   *
   * @return     The spatial velocity of the Frame expressed in the desired reference frame.
   *
   * @warning    Fist or second order forwardKinematics should have been called first
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameVelocity(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame rf = LOCAL)
  {
    typedef ModelTpl<Scalar, Options, JointCollectionTpl> Model;
    const typename Model::Frame & frame = model.frames[frame_id];

    return getFrameVelocity(model, data, frame.parentJoint, frame.placement, rf);
  }

  /**
   * @brief      Returns the spatial acceleration of the Frame expressed in the desired reference
   * frame. You must first call pinocchio::forwardKinematics to update placement, velocity and
   * acceleration values in data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  joint_id    Id of the parent joint
   * @param[in]  placement   frame placement with respect to the parent joint
   * @param[in]  rf          Reference frame in which the acceleration is expressed.
   *
   * @return The spatial acceleration of the Frame expressed in the desired reference frame.
   *
   * @warning    Second order forwardKinematics should have been called first
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameAcceleration(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const JointIndex joint_id,
    const SE3Tpl<Scalar, Options> & placement,
    const ReferenceFrame rf = LOCAL);

  /**
   * @brief      Returns the spatial acceleration of the Frame expressed in the desired reference
   * frame. You must first call pinocchio::forwardKinematics to update placement, velocity and
   * acceleration values in the data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  frame_id    Id of the operational Frame
   * @param[in]  rf          Reference frame in which the acceleration is expressed.
   *
   * @return The spatial acceleration of the Frame expressed in the desired reference frame.
   *
   * @warning    Second order @ref forwardKinematics should have been called first
   *
   * @remark     In the context of a frame placement constraint \f$J(q) a + \dot{J}(q, v) v = 0\f$,
   *             one way to compute the second term \f$\dot{J}(q, v) v\f$ is to call second-order
   * @ref forwardKinematics with a zero acceleration, then read the remaining \f$\dot{J}(q, v) v\f$
   * by calling this function. This is significantly more efficient than applying the matrix
   * \f$\dot{J}(q, v)\f$ (from @ref getFrameJacobianTimeVariation) to the velocity vector \f$v\f$.
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameAcceleration(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame rf = LOCAL)
  {
    typedef ModelTpl<Scalar, Options, JointCollectionTpl> Model;
    const typename Model::Frame & frame = model.frames[frame_id];
    return getFrameAcceleration(model, data, frame.parentJoint, frame.placement, rf);
  }

  /**
   * @brief      Returns the "classical" acceleration of the Frame expressed in the desired
   * reference frame. This is different from the "spatial" acceleration in that centrifugal effects
   * are accounted for. You must first call pinocchio::forwardKinematics to update placement,
   * velocity and acceleration values in data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  joint_id    Id of the parent joint
   * @param[in]  placement   frame placement with respect to the parent joint
   * @param[in]  rf          Reference frame in which the acceleration is expressed.
   *
   * @return The classical acceleration of the Frame expressed in the desired reference frame.
   *
   * @warning    Second order forwardKinematics should have been called first
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameClassicalAcceleration(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const JointIndex joint_id,
    const SE3Tpl<Scalar, Options> & placement,
    const ReferenceFrame rf = LOCAL);

  /**
   * @brief      Returns the "classical" acceleration of the Frame expressed in the desired
   * reference frame. This is different from the "spatial" acceleration in that centrifugal effects
   * are accounted for. You must first call pinocchio::forwardKinematics to update placement,
   * velocity and acceleration values in data structure.
   *
   * @param[in]  model       The kinematic model
   * @param[in]  data        Data associated to model
   * @param[in]  frame_id    Id of the operational Frame
   * @param[in]  rf          Reference frame in which the acceleration is expressed.
   *
   * @return The classical acceleration of the Frame expressed in the desired reference frame.
   *
   * @warning    Second order @ref forwardKinematics should have been called first
   *
   * @remark     In the context of a frame placement constraint \f$J(q) a + \dot{J}(q, v) v = 0\f$,
   *             one way to compute the second term \f$\dot{J}(q, v) v\f$ is to call second-order
   * @ref forwardKinematics with a zero acceleration, then read the remaining \f$\dot{J}(q, v) v\f$
   * by calling this function. This is significantly more efficient than applying the matrix
   * \f$\dot{J}(q, v)\f$ (from @ref getFrameJacobianTimeVariation) to the velocity vector \f$v\f$.
   *
   */

  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  inline MotionTpl<Scalar, Options> getFrameClassicalAcceleration(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame rf = LOCAL)
  {
    typedef ModelTpl<Scalar, Options, JointCollectionTpl> Model;
    const typename Model::Frame & frame = model.frames[frame_id];
    return getFrameClassicalAcceleration(model, data, frame.parentJoint, frame.placement, rf);
  }

  /**
   * @brief      Returns the jacobian of the frame given by its relative placement w.r.t. a joint
   * frame, and whose columns are either expressed in the LOCAL frame coordinate system, in the
   * local world aligned (rf = LOCAL_WORLD_ALIGNED) frame or in the WORLD coordinate system,
   * depending on the value of reference_frame. You must first call
   * pinocchio::computeJointJacobians.
   *
   * @remarks     Similarly to pinocchio::getJointJacobian:
   *                - if rf == LOCAL, this function returns the Jacobian of the frame expressed in
   * the local coordinate system of the frame
   *                - if rf == LOCAL_WORLD_ALIGNED, this function returns the Jacobian of the frame
   * centered on the frame origin and expressed in a coordinate system aligned with the WORLD.
   *                - if rf == WORLD, this function returns the Jacobian of the frame expressed at
   * the point coincident with the origin and expressed in a coordinate system aligned with the
   * WORLD.
   *
   * @tparam JointCollection Collection of Joint types.
   * @tparam Matrix6xLike Type of the matrix containing the joint Jacobian.
   *
   * @param[in]  model                        The kinematic model
   * @param[in]  data                           Data associated to model
   * @param[in]  joint_id                  Index of the joint.
   * @param[in]  reference_frame  Reference frame in which the Jacobian is expressed.
   * @param[out] J                                  The Jacobian of the Frame expressed in the
   * reference_frame coordinate system.
   *
   * @warning    The function pinocchio::computeJointJacobians should have been called first.
   */
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename Matrix6xLike>
  void getFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const JointIndex joint_id,
    const SE3Tpl<Scalar, Options> & placement,
    const ReferenceFrame reference_frame,
    const Eigen::MatrixBase<Matrix6xLike> & J);

  /**
   * @brief      Returns the jacobian of the frame given by its relative placement w.r.t. a joint
   * frame, and whose columns are either expressed in the LOCAL frame coordinate system, in the
   * local world aligned frame or in the WORLD coordinate system, depending on the value of
   * reference_frame. You must first call pinocchio::computeJointJacobians.
   *
   * @remarks     Similarly to pinocchio::getJointJacobian:
   *                - if rf == LOCAL, this function returns the Jacobian of the frame expressed in
   * the local coordinate system of the frame
   *                - if rf == LOCAL_WORLD_ALIGNED, this function returns the Jacobian of the frame
   * centered on the frame origin and expressed in a coordinate system aligned with the WORLD.
   *                - if rf == WORLD, this function returns the Jacobian of the frame expressed at
   * the point coincident with the origin and expressed in a coordinate system aligned with the
   * WORLD.
   *
   * @tparam JointCollection Collection of Joint types.
   *
   * @param[in]  model                        The kinematic model
   * @param[in]  data                           Data associated to model
   * @param[in]  joint_id                  Index of the joint.
   * @param[in]  reference_frame  Reference frame in which the Jacobian is expressed.
   *
   * @warning    The function pinocchio::computeJointJacobians should have been called first.
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  Eigen::Matrix<Scalar, 6, Eigen::Dynamic, Options> getFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const JointIndex joint_id,
    const SE3Tpl<Scalar, Options> & placement,
    const ReferenceFrame reference_frame)
  {
    typedef Eigen::Matrix<Scalar, 6, Eigen::Dynamic, Options> Matrix6x;
    Matrix6x res(Matrix6x::Zero(6, model.nv));

    getFrameJacobian(model, data, joint_id, placement, reference_frame, res);
    return res;
  }

  /**
   * @brief      Returns the jacobian of the frame expressed either expressed in the local frame
   * coordinate system, in the local world aligned frame or in the WORLD coordinate system,
   * depending on the value of reference_frame. You must first call
   * pinocchio::computeJointJacobians.
   *
   * @remarks     Similarly to pinocchio::getJointJacobian:
   *                - if rf == LOCAL, this function returns the Jacobian of the frame expressed in
   * the local coordinate system of the frame
   *                - if rf == LOCAL_WORLD_ALIGNED, this function returns the Jacobian of the frame
   * centered on the frame origin and expressed in a coordinate system aligned with the WORLD.
   *                - if rf == WORLD, this function returns the Jacobian of the frame expressed at
   * the point coincident with the origin and expressed in a coordinate system aligned with the
   * WORLD.
   *
   * @tparam JointCollection Collection of Joint types.
   * @tparam Matrix6xLike Type of the matrix containing the joint Jacobian.
   *
   * @param[in]  model                         The kinematic model
   * @param[in]  data                            Data associated to model
   * @param[in]  frame_id                   Index of the frame
   * @param[in]  reference_frame   Reference frame in which the Jacobian is expressed.
   * @param[out] J                                  The Jacobian of the Frame expressed in the
   * coordinates Frame.
   *
   * @warning    The function pinocchio::computeJointJacobians should have been called first.
   */
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename Matrix6xLike>
  inline void getFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame reference_frame,
    const Eigen::MatrixBase<Matrix6xLike> & J)
  {
    PINOCCHIO_CHECK_INPUT_ARGUMENT(
      frame_id < (FrameIndex)model.nframes, "The index of the Frame is outside the bounds.");

    typedef ModelTpl<Scalar, Options, JointCollectionTpl> Model;
    typedef typename Model::Frame Frame;

    const Frame & frame = model.frames[frame_id];
    data.oMf[frame_id] = data.oMi[frame.parentJoint] * frame.placement;

    getFrameJacobian(
      model, data, frame.parentJoint, frame.placement, reference_frame,
      PINOCCHIO_EIGEN_CONST_CAST(Matrix6xLike, J));
  }

  /**
   * @brief      Returns the jacobian of the frame expressed either expressed in the local frame
   * coordinate system, in the local world aligned frame or in the WORLD coordinate system,
   * depending on the value of reference_frame. You must first call
   * pinocchio::computeJointJacobians.
   *
   * @remarks     Similarly to pinocchio::getJointJacobian:
   *                - if rf == LOCAL, this function returns the Jacobian of the frame expressed in
   * the local coordinate system of the frame
   *                - if rf == LOCAL_WORLD_ALIGNED, this function returns the Jacobian of the frame
   * centered on the frame origin and expressed in a coordinate system aligned with the WORLD.
   *                - if rf == WORLD, this function returns the Jacobian of the frame expressed at
   * the point coincident with the origin and expressed in a coordinate system aligned with the
   * WORLD.
   *
   * @tparam JointCollection Collection of Joint types.
   *
   * @param[in]  model                         The kinematic model
   * @param[in]  data                            Data associated to model
   * @param[in]  frame_id                   Index of the frame
   * @param[in]  reference_frame   Reference frame in which the Jacobian is expressed.
   *
   * @warning    The function pinocchio::computeJointJacobians should have been called first.
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  Eigen::Matrix<Scalar, 6, Eigen::Dynamic, Options> getFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame reference_frame)
  {
    typedef Eigen::Matrix<Scalar, 6, Eigen::Dynamic, Options> Matrix6x;
    Matrix6x res(Matrix6x::Zero(6, model.nv));

    getFrameJacobian(model, data, frame_id, reference_frame, res);
    return res;
  }

  ///
  /// \brief Computes the Jacobian of a specific Frame expressed in the desired reference_frame
  /// given as argument.
  ///
  /// \tparam JointCollection Collection of Joint types.
  /// \tparam ConfigVectorType Type of the joint configuration vector.
  /// \tparam Matrix6xLike Type of the matrix containing the joint Jacobian.
  ///
  /// \param[in] model                                The model structure of the rigid body system.
  /// \param[in] data                                  The data structure of the rigid body system.
  /// \param[in] q                                         The joint configuration vector (dim
  /// model.nq). \param[in] frameId                            The id of the Frame refering to
  /// model.frames[frameId]. \param[in] reference_frame          Reference frame in which the
  /// Jacobian is expressed. \param[out] J                                       A reference on the
  /// Jacobian matrix where the results will be stored in (dim 6 x model.nv). You must fill J with
  /// zero elements, e.g. J.setZero().
  ///
  /// \return The Jacobian of the specific Frame expressed in the desired reference frame (matrix 6
  /// x model.nv).
  ///
  /// \remarks The result of this function is equivalent to call first
  /// computeJointJacobians(model,data,q), then updateFramePlacements(model,data) and then call
  /// getJointJacobian(model,data,jointId,rf,J),
  ///         but forwardKinematics and updateFramePlacements are not fully computed.
  ///
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename ConfigVectorType,
    typename Matrix6xLike>
  inline void computeFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const Eigen::MatrixBase<ConfigVectorType> & q,
    const FrameIndex frameId,
    const ReferenceFrame reference_frame,
    const Eigen::MatrixBase<Matrix6xLike> & J);

  ///
  /// \brief Computes the Jacobian of a specific Frame expressed in the LOCAL frame coordinate
  /// system.
  ///
  /// \tparam JointCollection Collection of Joint types.
  /// \tparam ConfigVectorType Type of the joint configuration vector.
  /// \tparam Matrix6xLike Type of the matrix containing the joint Jacobian.
  ///
  /// \param[in] model The model structure of the rigid body system.
  /// \param[in] data The data structure of the rigid body system.
  /// \param[in] q The joint configuration vector (dim model.nq).
  /// \param[in] frameId The id of the Frame refering to model.frames[frameId].
  ///
  /// \param[out] J A reference on the Jacobian matrix where the results will be stored in (dim 6 x
  /// model.nv). You must fill J with zero elements, e.g. J.setZero().
  ///
  /// \return The Jacobian of the specific Frame expressed in the LOCAL frame coordinate system
  /// (matrix 6 x model.nv).
  ///
  /// \remarks The result of this function is equivalent to call first
  /// computeJointJacobians(model,data,q), then updateFramePlacements(model,data) and then call
  /// getJointJacobian(model,data,jointId,LOCAL,J),
  ///         but forwardKinematics and updateFramePlacements are not fully computed.
  ///
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename ConfigVectorType,
    typename Matrix6xLike>
  inline void computeFrameJacobian(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const Eigen::MatrixBase<ConfigVectorType> & q,
    const FrameIndex frameId,
    const Eigen::MatrixBase<Matrix6xLike> & J)
  {
    computeFrameJacobian(
      model, data, q.derived(), frameId, LOCAL, PINOCCHIO_EIGEN_CONST_CAST(Matrix6xLike, J));
  }

  ///
  /// \brief Computes the Jacobian time variation of a specific frame (given by frame_id) expressed
  /// either in the WORLD frame (rf = WORLD), in the local world aligned (rf = LOCAL_WORLD_ALIGNED)
  /// frame or in the LOCAL frame (rf = LOCAL).
  ///
  /// \note This jacobian is extracted from data.dJ. You have to run
  /// pinocchio::computeJointJacobiansTimeVariation before calling it.
  ///
  /// \tparam JointCollection Collection of Joint types.
  /// \tparam Matrix6xLike Type of the matrix containing the joint Jacobian.
  ///
  /// \param[in] model The model structure of the rigid body system.
  /// \param[in] data The data structure of the rigid body system.
  /// \param[in] frameId The index of the frame.
  ///
  /// \param[out] dJ A reference on the Jacobian matrix where the results will be stored in (dim 6 x
  /// model.nv). You must fill dJ with zero elements, e.g. dJ.fill(0.).
  ///
  template<
    typename Scalar,
    int Options,
    template<typename, int> class JointCollectionTpl,
    typename Matrix6xLike>
  void getFrameJacobianTimeVariation(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    const ReferenceFrame rf,
    const Eigen::MatrixBase<Matrix6xLike> & dJ);

  /**
   * @brief Compute the inertia supported by a specific frame (given by frame_id) expressed in the
   * LOCAL frame. The total supported inertia corresponds to the sum of all the inertia after the
   * given frame, i.e :
   *         * The frame inertia
   *         * The child frames inertia ('Child frames' refers to frames that share the same parent
   * joint and are placed after the given frame)
   *         * The child joints inertia (if with_subtree == true)
   *        You must first call pinocchio::forwardKinematics to update placement values in data
   * structure.
   *
   * @note Physically speaking, if the robot were to be cut in two parts at that given frame, this
   * supported inertia would represents the inertia of the part that was after the frame.
   *       with_subtree determines if the childs joints must be taken into consideration (if true)
   * or only the current joint (if false).
   *
   * @note The equivalent function for a joint would be :
   *       * to read `data.Ycrb[joint_id]`, after having called pinocchio::crba (if with_subtree ==
   * true).
   *       * to read `model.inertia[joint_id]` (if with_subtree == false).
   *
   * @tparam JointCollection Collection of Joint types.
   *
   * @param[in] model The model structure of the rigid body system.
   * @param[in] data The data structure of the rigid body system.
   * @param[in] frameId The index of the frame.
   * @param[in] with_subtree If false, compute the inertia only inside the frame parent joint if
   * false. If true, include child joints inertia.
   *
   * @return The computed inertia.
   *
   * @warning forwardKinematics should have been called first
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  InertiaTpl<Scalar, Options> computeSupportedInertiaByFrame(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id,
    bool with_subtree);

  /**
   * @brief Computes the force supported by a specific frame (given by frame_id) expressed in the
   * LOCAL frame. The supported force corresponds to the sum of all the forces experienced after the
   * given frame, i.e :
   *         * The inertial forces and gravity (applied on the supported inertia in body)
   *         * The forces applied by child joints
   *         * (The external forces)
   *        You must first call pinocchio::rnea to update placements, velocities and efforts values
   * in data structure.
   *
   * @note If an external force is applied to the frame parent joint (during rnea), it won't be
   * taken in consideration in this function (it will be considered to be applied before the frame
   * in the joint and not after. However external forces applied to child joints will be taken into
   * account).
   *
   * @note Physically speaking, if the robot were to be separated in two parts glued together at
   * that given frame, the supported force represents the internal forces applide from the part
   * after the cut/frame to the part before. This compute what a force-torque sensor would measures
   * if it would be placed at that frame.
   *
   * @note The equivalent function for a joint would be to read `data.f[joint_id]`, after having
   * call pinocchio::rnea.
   *
   * @tparam JointCollection Collection of Joint types.
   *
   * @param[in] model The model structure of the rigid body system.
   * @param[in] data The data structure of the rigid body system.
   * @param[in] frameId The index of the frame.
   *
   * @return The computed force.
   *
   * @warning pinocchio::rnea should have been called first
   */
  template<typename Scalar, int Options, template<typename, int> class JointCollectionTpl>
  ForceTpl<Scalar, Options> computeSupportedForceByFrame(
    const ModelTpl<Scalar, Options, JointCollectionTpl> & model,
    const DataTpl<Scalar, Options, JointCollectionTpl> & data,
    const FrameIndex frame_id);

} // namespace pinocchio

/* --- Details -------------------------------------------------------------------- */
#include "pinocchio/algorithm/frames.hxx"

#if PINOCCHIO_ENABLE_TEMPLATE_INSTANTIATION
  #include "pinocchio/algorithm/frames.txx"
#endif // PINOCCHIO_ENABLE_TEMPLATE_INSTANTIATION

#endif // ifndef __pinocchio_algorithm_frames_hpp__
