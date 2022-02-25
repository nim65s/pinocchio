//
// Copyright (c) 2022 INRIA
//

#ifndef __pinocchio_multibody_tree_broadphase_manager_hpp__
#define __pinocchio_multibody_tree_broadphase_manager_hpp__

#ifdef PINOCCHIO_WITH_HPP_FCL

#include "pinocchio/multibody/broadphase-manager.hpp"

namespace pinocchio
{

template<typename _Manager>
struct TreeBroadPhaseManagerTpl
: public BroadPhaseManagerBase< TreeBroadPhaseManagerTpl<_Manager> >
{
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  typedef _Manager Manager;
  typedef BroadPhaseManagerBase< TreeBroadPhaseManagerTpl<_Manager> > Base;
  typedef BroadPhaseManagerTpl<Manager> BroadPhaseManager;
  
  typedef std::vector<hpp::fcl::CollisionObject *> CollisionObjectPointerVector;
  typedef std::vector<BroadPhaseManager> BroadPhaseManagerVector;

  /// @brief Default constructor.
  TreeBroadPhaseManagerTpl() // for std::vector
  : Base()
  {}
  
  /// @brief Constructor from a given geometry model and data.
  ///
  /// \param[in] model_ptr pointer to the model of the kinematic tree.
  /// \param[in] geometry_model_ptr pointer to the geometry model.
  /// \param[in] geometry_data_ptr pointer to the geometry data.
  ///
  TreeBroadPhaseManagerTpl(const Model * model_ptr,
                           const GeometryModel * geometry_model_ptr,
                           GeometryData * geometry_data_ptr)
  : Base(model_ptr, geometry_model_ptr, geometry_data_ptr)
  {
    init(static_cast<size_t>(model_ptr->njoints));
  }
  
  /// @brief Copy contructor.
  ///
  /// \param[in] other manager to copy.
  ///
  TreeBroadPhaseManagerTpl(const TreeBroadPhaseManagerTpl & other)
  : Base(other)
  {
    init(other.managers.size());
  }
  
  using Base::getModel;
  using Base::getGeometryModel;
  using Base::getGeometryData;
  
  ///
  /// @brief Update the manager from the current geometry positions and update the underlying FCL broad phase manager.
  ///
  /// @param[in] compute_local_aabb whether to recompute the local AABB of the collision geometries which have changed.
  ///
  void update(bool compute_local_aabb = false);
  
  ///
  /// @brief Update the manager with a new geometry data.
  ///
  /// \param[in] geom_data_ptr_new pointer to the new geometry data.
  ///
  void update(GeometryData * geom_data_ptr_new);
  
  ~TreeBroadPhaseManagerTpl() {}
  
  /// @brief Check whether the base broad phase manager is aligned with the current collision_objects.
  bool check() const;
  
  /// @brief Check whether the callback is inline with *this
  bool check(CollisionCallBackBase * callback) const;
  
  /// @brief Performs collision test between one object and all the objects belonging to the manager.
  bool collide(CollisionObject & obj, CollisionCallBackBase * callback) const;
  
  /// @brief Performs collision test for the objects belonging to the manager.
  bool collide(CollisionCallBackBase * callback) const;
  
  /// @brief Performs collision test with objects belonging to another manager.
  bool collide(TreeBroadPhaseManagerTpl & other_manager, CollisionCallBackBase * callback) const;

//  /// @brief Performs distance computation between one object and all the objects belonging to the manager
//  void distance(CollisionObject* obj, DistanceCallBackBase * callback) const;

//  /// @brief Performs distance test for the objects belonging to the manager (i.e., N^2 self distance)
//  void distance(DistanceCallBackBase * callback) const;
  
//  /// @brief Performs distance test with objects belonging to another manager
//  void distance(TreeBroadPhaseManagerTpl* other_manager, DistanceCallBackBase * callback) const;
  
protected:
  
  /// @brief the vector of collision objects.
  BroadPhaseManagerVector managers;
  
  /// @brief Initialialisation
  void init(const size_t njoints);
  
}; // struct BroadPhaseManagerTpl<BroadPhaseManagerDerived>

} // namespace pinocchio

/* --- Details -------------------------------------------------------------------- */
#include "pinocchio/multibody/tree-broadphase-manager.hxx"

#endif // ifdef PINOCCHIO_WITH_HPP_FCL

#endif // ifndef __pinocchio_multibody_tree_broadphase_manager_hpp__
