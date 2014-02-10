#ifndef BOOLEANSIGNALTRANSITION_H
#define BOOLEANSIGNALTRANSITION_H

#include <QSignalTransition>

class BooleanSignalTransition : public QSignalTransition
{
    Q_OBJECT
    BooleanSignalTransition( QState* sourceState, const char* signal, QAbstractState* targetState )
        : QSignalTransition( sourceState, signal, targetState ),
          m_test( true )
    {}

    void setTest( bool b ) { m_test = b; }
    bool test() const { return m_test; }

    virtual bool eventTest( QEvent* e )
    {
        // Let QSignalTransition check it is the correct signal first of all
        if ( !QSignalTransition::eventTest( e ) )
            return false;

        // Now check that the argument has the correct value
        QStateMachine::SignalEvent* se = static_cast<QStateMachine::SignalEvent*>( e );
        bool value = se->arguments().at( 0 ).value<bool>();
        return ( value == m_test );
    }

private:
    bool m_test;
};

#endif // BOOLEANSIGNALTRANSITION_H
