#ifndef MINIFSM_MINI_FSM_H
#define MINIFSM_MINI_FSM_H

#include <vector>
#include <memory>

namespace minifsm {
namespace internal {
    struct StateModel {
        virtual long getId() const = 0;
        virtual bool isFinal() const = 0;
        virtual ~StateModel();
    };
    StateModel::~StateModel() {}

    using State = std::shared_ptr<StateModel>;

    template<long ID, bool FINAL>
    struct TemplatedStateModel : public internal::StateModel {
        long getId() const override;
        bool isFinal() const override;

        static State make();
    };

    template<typename T>
    struct SymbolModel {
        virtual T getId() const = 0;
        virtual ~SymbolModel();
    };

    template<typename T>
    SymbolModel<T>::~SymbolModel() {}

    template<typename T>
    using Symbol = std::shared_ptr<SymbolModel<T>>;

    template<typename T, T ID>
    struct TemplatedSymbolModel : public SymbolModel<T> {
        T getId() const override;

        static Symbol<T> make();
    };
}
    using State = internal::State;
    using Symbol = internal::Symbol<long>;

    struct Transition {
        State m_source;
        Symbol m_symbol;
        State m_target;
    };

    using Delta = std::vector<Transition>;

    template<long ID> struct SimpleState : public internal::TemplatedStateModel<ID, false> {};
    template<long ID> struct FinalState : public internal::TemplatedStateModel<ID, true> {};
    template<long ID> struct SimpleSymbol : public internal::TemplatedSymbolModel<long, ID> {};

    class FSM {
    public:
        FSM(const State &t_q0, const Delta &t_delta);
        bool transit(const Symbol &t_symbol);
        bool transit(const std::initializer_list<Symbol> &t_symbolSequence);
        State getCurrentState() const;
        bool isInFinalState() const;

    private:
        State m_q0;
        State m_currentState;
        Delta m_delta;
    };
}

#ifdef MINIFSM_IMPLEMENTATION
#undef MINIFSM_IMPLEMENTATION

namespace minifsm {

    FSM::FSM(const State &t_q0, const Delta &t_delta)
        : m_q0(t_q0), m_delta(t_delta), m_currentState(t_q0) {
        for(const auto &transition0 : m_delta) {
            for (const auto &transition1 : m_delta) {
                if( transition0.m_source == transition1.m_source &&
                    transition0.m_symbol == transition1.m_symbol &&
                    transition0.m_target != transition1.m_target) {
                    std::cout << "NON DETERMINISTIC DETECTED!" << '\n';
                }
            }
        }
    }

    bool FSM::transit(const Symbol &t_symbol) {
        for(const auto &transition : this->m_delta) {
            if(transition.m_source == m_currentState && transition.m_symbol == t_symbol) {
                m_currentState = transition.m_target;
                return true;
            }
        }
        return false;
    }

    bool FSM::transit(const std::initializer_list<Symbol> &t_symbolSequence) {
        bool allTransitionsDone = true;
        for(const auto &symbol : t_symbolSequence) {
            if(!this->transit(symbol)) allTransitionsDone = false;
        }
        return allTransitionsDone;
    }

    State FSM::getCurrentState() const {
        return m_currentState;
    }

    bool FSM::isInFinalState() const {
        return getCurrentState()->isFinal();
    }


    namespace internal {
    template<long ID, bool FINAL> long TemplatedStateModel<ID, FINAL>::getId() const { return ID; }
    template<long ID, bool FINAL> bool TemplatedStateModel<ID, FINAL>::isFinal() const { return FINAL; }
    template<long ID, bool FINAL> State TemplatedStateModel<ID, FINAL>::make() {
        return std::make_shared<TemplatedStateModel<ID, FINAL>>();
    }
    template<typename T, T ID>
    T TemplatedSymbolModel<T, ID>::getId() const {
        return ID;
    }

    template<typename T, T ID>
    Symbol<T> TemplatedSymbolModel<T, ID>::make() {
        return std::make_shared<TemplatedSymbolModel<T, ID>>();
    }
}
}

#endif
#endif //MINIFSM_MINI_FSM_H
