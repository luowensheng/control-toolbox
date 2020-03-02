/**********************************************************************************************************************
This file is part of the Control Toolbox (https://github.com/ethz-adrl/control-toolbox), copyright by ETH Zurich.
Licensed under the BSD-2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

#pragma once

#include "Controller.h"
#include <ct/core/control/discrete_time/DiscreteController.h>

namespace ct {
namespace core {

//! A constant controller
/*!
 * Implements a controller that is time and state invariant, i.e. fully constant.
 * This class is useful to integrate a ControlledSystem forward subject to a
 * constant control input.
 */
template <typename MANIFOLD, size_t CONTROL_DIM, typename SCALAR = typename MANIFOLD::Scalar>
class ConstantController final : public Controller<MANIFOLD, CONTROL_DIM, SCALAR>,
                                 public DiscreteController<MANIFOLD, CONTROL_DIM, SCALAR>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    //! Default constructor
    /*!
     * Sets the control signal to zero
     */
    ConstantController();

    //! Constructor
    /*!
     * Initializes the control to a fixed value
     * @param u The fixed control signal
     */
    ConstantController(ControlVector<CONTROL_DIM, SCALAR>& u);

    //! Copy constructor
    ConstantController(const ConstantController<MANIFOLD, CONTROL_DIM, SCALAR>& other);

    //! Destructor
    virtual ~ConstantController() = default;

    //! Clone operator
    /*!
     * Clones the controller. Used for cloning ControlledSystem's
     * @return pointer to cloned controller
     */
    ConstantController<MANIFOLD, CONTROL_DIM, SCALAR>* clone() const override;

    //! Computes current control
    /*!
     * Returns the fixed control signal. Therefore, the return value is invariant
     * to the parameters.
     * @param state The current state of the system (ignored)
     * @param t The time of the system (ignored)
     * @param controlAction The (fixed) control action
     */
    void computeControl(const MANIFOLD& state,
        const SCALAR& t,
        ControlVector<CONTROL_DIM, SCALAR>& controlAction) override;

    //! Equivalent method for discrete version
    void computeControl(const MANIFOLD& state, const int n, ControlVector<CONTROL_DIM, SCALAR>& controlAction) override;

    //! Sets the control signal
    /*!
     *
     * @param u The fixed control signal
     */
    void setControl(const ControlVector<CONTROL_DIM, SCALAR>& u);

    //! Get the fixed control signal
    /*!
     *
     * @param u The control input to write the signal to.
     */
    const ControlVector<CONTROL_DIM, SCALAR>& getControl() const;

    virtual ControlMatrix<CONTROL_DIM, SCALAR> getDerivativeU0(const MANIFOLD& state, const SCALAR time) override;

private:
    ControlVector<CONTROL_DIM, SCALAR> u_;
};
}  // namespace core
}  // namespace ct
