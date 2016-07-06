/*
 * SymbolicListener.h
 *
 *  Created on: 2015年11月13日
 *      Author: zhy
 */

#ifndef LIB_CORE_SYMBOLICLISTENER_H_
#define LIB_CORE_SYMBOLICLISTENER_H_

#include "klee/ExecutionState.h"
#include "klee/Expr.h"
#include "klee/util/Ref.h"
#include "../Core/AddressSpace.h"
#include "../Thread/StackType.h"
#include "BitcodeListener.h"
#include "Event.h"
#include "FilterSymbolicExpr.h"
#include "../Encode/RuntimeDataManager.h"
#include "../Core/Executor.h"

#include <map>
#include <string>
#include <vector>

namespace llvm {
	class Type;
	class Constant;
}

namespace klee {

	class SymbolicListener: public BitcodeListener {
		public:
			SymbolicListener(Executor* executor, RuntimeDataManager* rdManager);
			virtual ~SymbolicListener();

			void beforeRunMethodAsMain(ExecutionState &initialState);
			void beforeExecuteInstruction(ExecutionState &state, KInstruction *ki);
			void afterExecuteInstruction(ExecutionState &state, KInstruction *ki);
			void afterRunMethodAsMain();
			void executionFailed(ExecutionState &state, KInstruction *ki);

		private:
			Executor* executor;
			RuntimeDataManager* runtimeData;
			FilterSymbolicExpr filter;

			std::vector<Event*>::iterator currentEvent, endEvent;

			//此Map更新有两处，Load、某些函数。
			std::map<ref<Expr>, ref<Expr> > addressSymbolicMap;
			std::map<std::string, std::vector<unsigned> > assertMap;
			std::map<std::string, ref<Expr> > symbolicMap;
			bool kleeBr;

			AddressSpace addressSpace;
			std::map<unsigned, StackType *> stack;

		private:

			//add by hy
			ref<Expr> manualMakeSymbolic(ExecutionState& state, std::string name, unsigned size, bool isFloat);
			void storeZeroToExpr(ExecutionState& state, ref<Expr> address, Expr::Width type);
			ref<Expr> readExpr(ExecutionState &state, ref<Expr> address, Expr::Width size);

	};

}

#endif /* LIB_CORE_SYMBOLICLISTENER_H_ */
