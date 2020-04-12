/**********************************************************************************************************************
This file is part of the Control Toolbox (https://github.com/ethz-adrl/control-toolbox), copyright by ETH Zurich.
Licensed under the BSD-2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

#pragma once

#include <ct/optcon/constraint/LinearConstraintContainer.h>
#include <ct/optcon/costfunction/CostFunctionQuadratic.hpp>

namespace ct {
namespace optcon {


/*!
 * \defgroup OptConProblem OptConProblem
 *
 * \brief Class that defines how to set up an Optimal Control Problem
 *
 * An finite-horizon optimal control problem is generally defined through
 * 	- nonlinear system dynamics
 * 	- cost function (intermediate + terminal cost)
 * 	- initial state
 * 	- box constraints
 * 	- general constraints
 * 	- an overall time horizon
 *
 * 	Note that in most cases, the user can also provide a pointer to the linearized system dynamics. This is optional, and
 * 	in case it is not provided, numerical differentiation will be applied to approximate the linearized dynamics.
 *
 * 	\warning Using numerical differentiation is inefficient and typically slow.
 *
 */
template <typename MANIFOLD, size_t CONTROL_DIM, ct::core::TIME_TYPE TIME_T>
class OptConProblem
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    using SCALAR = typename MANIFOLD::Scalar;

    static constexpr size_t STATE_DIM = MANIFOLD::TangentDim;

    using ControlledSystem_t = ct::core::ControlledSystem<MANIFOLD, CONTROL_DIM, TIME_T>;
    using DynamicsPtr_t = std::shared_ptr<ct::core::ControlledSystem<MANIFOLD, CONTROL_DIM, TIME_T>>;

    using LinearSystem_t = ct::core::LinearSystem<MANIFOLD, CONTROL_DIM, TIME_T>;
    using LinearPtr_t = std::shared_ptr<LinearSystem_t>;

    using CostFunctionPtr_t = std::shared_ptr<optcon::CostFunctionQuadratic<MANIFOLD, CONTROL_DIM>>;
    // using ConstraintPtr_t = std::shared_ptr<optcon::LinearConstraintContainer<STATE_DIM, CONTROL_DIM, SCALAR>>; // TODO

    using Time_t typename ControlledSystem_t::Time_t;

    OptConProblem() = default;  // TODO

    /*!
     * @brief Construct a simple unconstrained Optimal Control Problem
     * \warning time and initial state to be specified later
     *
     * @param nonlinDynamics the nonlinear system dynamics
     * @param costFunction a quadratic cost function
     * @param linearSystem (optional) the linear system holding the dynamics derivatives. If the
     * user does not specify the derivatives, they are generated automatically using numerical differentiation. Warning: this is slow
     */
    OptConProblem(DynamicsPtr_t nonlinDynamics, CostFunctionPtr_t costFunction, LinearPtr_t linearSystem = nullptr);

    /*!
     * @brief Construct a simple unconstrained optimal control problem, with initial state and final time as constructor arguments
     * @param tf The optimal control problem final time horizon
     * @param x0 The initial system state
     * @param nonlinDynamics The nonlinear system dynamics
     * @param costFunction A quadratic cost function
     * @param linearSystem (optional) Linearized System Dynamics.
     */
    OptConProblem(const Time_t tf,
        const MANIFOLD& x0,
        DynamicsPtr_t nonlinDynamics,
        CostFunctionPtr_t costFunction,
        LinearPtr_t linearSystem = nullptr);

    /*!
     * @brief Construct a constrained Optimal Control Problem
     *
     * @param nonlinDynamics the nonlinear system dynamics
     * @param costFunction a quadratic cost function
     * @param inputBoxConstraints the input box constraints
     * @param stateBoxConstraints the state box constraints
     * @param generalConstraints the general constraints
     * @param linearSystem (optional) the linear system holding the dynamics derivatives.
     *
     * \warning time and initial state to be specified later
     * \warning If the user does not specify the derivatives, they are generated automatically using numerical differentiation. This is slow
     */
    //OptConProblem(DynamicsPtr_t nonlinDynamics, // TODO: bring back this constructor
    //    CostFunctionPtr_t costFunction,
    //    ConstraintPtr_t inputBoxConstraints,
    //    ConstraintPtr_t stateBoxConstraints,
    //    ConstraintPtr_t generalConstraints,
    //    LinearPtr_t linearSystem = nullptr);

    /*!
     * @brief Construct a constrained Optimal Control Problem
     *
     * @param tf The optimal control problem final time horizon
     * @param x0 The initial system state
     * @param nonlinDynamics the nonlinear system dynamics
     * @param costFunction a quadratic cost function
     * @param inputBoxConstraints the input box constraints
     * @param stateBoxConstraints the state box constraints
     * @param generalConstraints the general constraints
     * @param linearSystem (optional) the linear system holding the dynamics derivatives.
     *
     * \warning time and initial state to be specified later
     * \warning If the user does not specify the derivatives, they are generated automatically using numerical differentiation. This is slow
     */
    //OptConProblem(const Time_t tf, // TODO: bring back this constructor
    //    const MANIFOLD& x0,
    //    DynamicsPtr_t nonlinDynamics,
    //    CostFunctionPtr_t costFunction,
    //    ConstraintPtr_t inputBoxConstraints,
    //    ConstraintPtr_t stateBoxConstraints,
    //    ConstraintPtr_t generalConstraints,
    //    LinearPtr_t linearSystem = nullptr);

    //! check if all the ingredients for an unconstrained optimal control problem are there
    void verify() const;

    /*!
     * returns a pointer to the controlled system
     */
    const DynamicsPtr_t getNonlinearSystem() const;

    /*!
     * returns a pointer to the linear system approximation
     */
    const LinearPtr_t getLinearSystem() const;

    /*!
     * returns a pinter to the cost function
     */
    const CostFunctionPtr_t getCostFunction() const;

    /*!
     * returns a pointer to the controlled system
     */
    void setNonlinearSystem(const DynamicsPtr_t dyn);

    /*!
     * returns a pointer to the linear system approximation
     */
    void setLinearSystem(const LinearPtr_t lin);

    /*!
     * returns a pinter to the cost function
     */
    void setCostFunction(const CostFunctionPtr_t cost);

    /*!
     * set input box constraints
     * @param constraint pointer to box constraint
     */
    //void setInputBoxConstraints(const ConstraintPtr_t constraint); // TODO: bring back this method

    /*!
     * set state box constraints
     * @param constraint pointer to box constraint
     */
    //void setStateBoxConstraints(const ConstraintPtr_t constraint);  // TODO: bring back this method

    /*!
     * set general constraints
     * @param constraint pointer to a general constraint
     */
    // void setGeneralConstraints(const ConstraintPtr_t constraint);  // TODO: bring back this method

    /**
     * @brief      Retrieve the input box constraints
     *
     * @return     The input box constraints.
     */
    // const ConstraintPtr_t getInputBoxConstraints() const;  // TODO: bring back this method

    /**
     * @brief      Retrieve the state box constraints
     *
     * @return     The state box constraints.
     */
    // const ConstraintPtr_t getStateBoxConstraints() const;  // TODO: bring back this method

    /**
     * @brief      Retrieves the general constraints
     *
     * @return     The the general constraints
     */
    // const ConstraintPtr_t getGeneralConstraints() const;  // TODO: bring back this method

    /*!
     * get initial state (called by solvers)
     */
    const MANIFOLD getInitialState() const;

    /*!
     * set initial state for first subsystem
     */
    void setInitialState(const MANIFOLD& x0);

    /*!
     * get the current time horizon
     * @return	Time Horizon
     */
    Time_t getTimeHorizon() const;

    /*!
     * Update the current time horizon in the Opt.Control Problem (required for example for replanning)
     * @param tf new time horizon
     */
    void setTimeHorizon(const Time_t tf);


private:
    Time_t tf_;  //! end time

    MANIFOLD x0_;  //! initial state

    DynamicsPtr_t controlledSystem_;  //! the nonlinear system
    CostFunctionPtr_t costFunction_;  //! a quadratic cost function
    LinearPtr_t linearizedSystem_;    //! the linear approximation of the nonlinear system

    /*!
     * @brief container for input box constraints of the problem
     * Expected form:
     * \f$ u_{lb} \leq u \leq u_{ub} \f$
     */
    // ConstraintPtr_t inputBoxConstraints_;  // TODO: bring back this member

    /*!
     * @brief container for state box constraints of the problem
     * Expected form:
     * \f$ x_{lb} \leq x \leq x_{ub} \f$
     */
    // ConstraintPtr_t stateBoxConstraints_; // TODO: bring back this member

    /*!
     * @brief container of all the general constraints of the problem
     * Expected form:
     * \f$ d_{lb} \leq g(x,u) \leq d_{ub} \f$
     */
    // ConstraintPtr_t generalConstraints_; // TODO: bring back this member
};

}  // namespace optcon
}  // namespace ct
