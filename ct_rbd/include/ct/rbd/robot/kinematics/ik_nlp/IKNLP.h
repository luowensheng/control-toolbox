/**********************************************************************************************************************
This file is part of the Control Toolbox (https://adrlab.bitbucket.io/ct), copyright by ETH Zurich, Google Inc.
Licensed under Apache2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

#pragma once

#include <ct/optcon/nlp/Nlp>

#include "IKCostEvaluator.h"
#include "IKConstraintContainer.h"

namespace ct {
namespace rbd {

/*!
 * todo: set more meaningful initial guess
 */
template <typename KINEMATICS, typename SCALAR = double>
class IKNLP : public ct::optcon::tpl::Nlp<SCALAR>
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	using Scalar_t = SCALAR;
	using Kinematics_t = KINEMATICS;

    using InverseKinematicsBase = ct::rbd::InverseKinematicsBase<KINEMATICS::NJOINTS, SCALAR>;
    using JointPosition_t = typename InverseKinematicsBase::JointPosition_t;
    using JointPositionsVector_t = typename InverseKinematicsBase::JointPositionsVector_t;
    using RigidBodyPoseTpl = typename InverseKinematicsBase::RigidBodyPoseTpl;

    //! constructor
    IKNLP(std::shared_ptr<ct::rbd::IKCostEvaluator<KINEMATICS, SCALAR>> costEvaluator,
        const JointPosition_t& lowerBound,
        const JointPosition_t& upperBound)
    {
        // the number of optimization variables is the number of robot joints
        this->optVariables_ = std::shared_ptr<ct::optcon::tpl::OptVector<SCALAR>>(
            new ct::optcon::tpl::OptVector<SCALAR>(KINEMATICS::NJOINTS));
        // trivial initial guess
        this->optVariables_->setZero();

        // set the cost evaluator
        this->costEvaluator_ = costEvaluator;
        std::static_pointer_cast<ct::rbd::IKCostEvaluator<KINEMATICS, SCALAR>>(this->costEvaluator_)
            ->setOptVector(this->optVariables_);

        this->constraints_ = std::shared_ptr<IKConstraintsContainer<KINEMATICS, SCALAR>>(
            new IKConstraintsContainer<KINEMATICS, SCALAR>(this->optVariables_, lowerBound, upperBound));
    }

    //! default destructor
    virtual ~IKNLP() override = default;

    virtual void updateProblem() override { /* do nothing */}

    JointPosition_t getSolution()
    {
    	return this->optVariables_->getOptimizationVars();
    }

    //! print the solution to command line
    void printSolution() const
    {
        std::cout << "IKNLP Solution: " << std::endl
                  << this->optVariables_->getOptimizationVars().transpose() << std::endl;
    }

    std::shared_ptr<ct::rbd::IKCostEvaluator<KINEMATICS, SCALAR>> getIKCostEvaluator()
    {
    	return std::static_pointer_cast<ct::rbd::IKCostEvaluator<KINEMATICS, SCALAR>>(this->costEvaluator_);
    }

    void setInitialGuess(const JointPosition_t& q_init)
    {
    	this->optVariables_->setInitialGuess(q_init);
    }

private:
};

}  // rbd
}  // ct
