#ifndef MINIFSM_MINI_FSM_H
#define MINIFSM_MINI_FSM_H

#include <memory>
#include <vector>

namespace minifsm {

    class StateModel {
    public:
        virtual long getId() const = 0;
        virtual bool isFinal() const = 0;
        virtual ~StateModel();
    };
    using State = std::shared_ptr<StateModel>;

    class SymbolModel {
    public:
        virtual long getId() const = 0;
    };
    using Symbol = std::shared_ptr<SymbolModel>;

    template<long ID, bool FINAL>
    class TemplatedStateModel : public StateModel {
    public:
        long getId() const override;
        bool isFinal() const override;
        static State make();
    };

    template<long ID>
    class TemplatedSymbolModel : public SymbolModel {
    public:
        long getId() const override;
        static Symbol make();
    };

    struct Transition {
        State source;
        Symbol symbol;
        State target;
    };

    using Delta = std::vector<Transition>;

    class FSM {
    public:
        FSM(const State &t_s0, const Delta &t_delta);

        bool transit(const Symbol &t_symbol);
        bool transit(const std::initializer_list<Symbol> &t_symbolsequence);
        bool accepts(const std::initializer_list<Symbol> &t_symbolsequence);

    private:
        State s0;
        State current_state;
        Delta delta;
    };

}

#ifdef MINIFSM_IMPLEMENTATION
#undef MINIFSM_IMPLEMENTATION

namespace minifsm {
    FSM::FSM(const State &t_s0, const Delta &t_delta) : s0(t_s0), delta(t_delta), current_state(t_s0) {
        // check delta
        for(const auto &transition0 : delta) {
            for (const auto &transition1 : delta) {
                if(transition0.source == transition1.source && transition0.symbol == transition1.symbol && transition0.target != transition1.target) {
                    std::cout << "NON DETERMINISTIC DETECTED!" << '\n';
                }
            }
        }
    }

    bool FSM::accepts(const std::initializer_list<Symbol> &t_symbolsequence) {
        return transit(t_symbolsequence);
    }

    bool FSM::transit(const std::initializer_list<Symbol> &t_symbolsequence) {
        bool allTransitionsDone = true;
        for(const auto &symbol : t_symbolsequence) {
            if(!this->transit(symbol)) allTransitionsDone = false;
        }
        return allTransitionsDone;
    }

    bool FSM::transit(const Symbol &t_symbol) {
        for(const auto &transition : this->delta) {
            if(transition.source == current_state && transition.symbol == t_symbol) {
                current_state = transition.target;
                return true;
            }
        }
        return false;
    }

    StateModel::~StateModel() {}

    template<long ID, bool FINAL>
    long TemplatedStateModel<ID, FINAL>::getId() const {
        return ID;
    }

    template<long ID, bool FINAL>
    bool TemplatedStateModel<ID, FINAL>::isFinal() const {
        return FINAL;
    }

    template<long ID, bool FINAL>
    State TemplatedStateModel<ID, FINAL>::make() {
        return std::make_shared<TemplatedStateModel<ID, FINAL>>();
    }

    template<long ID>
    long TemplatedSymbolModel<ID>::getId() const {
        return ID;
    }
    template<long ID>
    Symbol TemplatedSymbolModel<ID>::make() {
        return std::make_shared<TemplatedSymbolModel<ID>>();
    }

}

#endif
#endif //MINIFSM_MINI_FSM_H
