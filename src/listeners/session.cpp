#include "session.h"

#include "session/logind.h"

SessionListener::SessionListener(QObject* parent) : QObject(parent),
    m_impl(new LogindListener(this))
{
    QObject::connect((LogindListener*)m_impl, &LogindListener::userLoggedIn, this, &SessionListener::userLoggedIn);
    QObject::connect((LogindListener*)m_impl, &LogindListener::userLoggedOut, this, &SessionListener::userLoggedOut);
}
