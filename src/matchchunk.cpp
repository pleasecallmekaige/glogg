#include "matchchunk.h"
#include "log.h"

// return Relative_Position betwin old MatchChunk and new MatchChunk
static inline Relative_Position _Get_Position(const MatchChunk& Item,const MatchChunk& object){
    Q_ASSERT(object.length() != 0);
    Q_ASSERT(object.startColumn() != object.endColumn());
    if ( object.startColumn() <= Item.startColumn() ) {
        if ( object.endColumn() <= Item.startColumn() ) {
            return BEFORE;
        } else if ( object.endColumn() > Item.startColumn() && object.endColumn() < Item.endColumn()) {
            return BERORE_INTERSERT;
        } else if ( object.endColumn() >= Item.endColumn() ) {
            return CONTAIN;
        } else {
            Q_ASSERT(0);
        }
    } else if ( object.startColumn() > Item.startColumn() && object.startColumn() < Item.endColumn() ) {
        if ( object.endColumn() < Item.endColumn()) {
            return BE_CONTAIN;
        } else if ( object.endColumn() >= Item.endColumn() ) {
            return AFTER_INTERSERT;
        } else {
            Q_ASSERT(0);
        }
    } else if ( object.startColumn() >= Item.endColumn() ) {
        return AFTER;
    } else {
        Q_ASSERT(0);
        return AFTER;
    }
}

QLinkedList<MatchChunk>& MatchChunk::addToList( QLinkedList<MatchChunk>& out)
{
    QLinkedList<MatchChunk>::iterator i;

    if ( out.isEmpty() || this->startColumn() >= out.last().endColumn() ) {
        out.push_back(*this);
        return out;
    }

    for (i = out.begin(); i != out.end();) {
        switch ( _Get_Position( *i, *this) ) {
            case BEFORE:
                out.insert(i, *this);
                // LOG(logINFO) << "INSERT:" << "start:" << this->startColumn() << " end:" << this->endColumn();
                return out;
            break;
            case AFTER:
                ++i;
            break;
            case BERORE_INTERSERT:
                i->setStartColumn(this->endColumn());
            break;
            case AFTER_INTERSERT:
                i->setEndColumn(this->startColumn());
                ++i;
            break;
            case CONTAIN:
                i = out.erase(i);
            break;
            case BE_CONTAIN:
                // 原本的MatchChunk被拆成了两半
                // LOG(logINFO) << "BE_CONTAIN: newMatch" << "start:" << this->endColumn() << " end:" << i->endColumn();
                MatchChunk newMatch (*i);
                newMatch.setStartColumn( this->endColumn() );
                i->setEndColumn(this->startColumn());
                if ( i+1 == out.end() ) {
                    // LOG(logINFO) << "BE_CONTAIN:push_back";
                    out.push_back(newMatch);
                } else {
                    // LOG(logINFO) << "BE_CONTAIN:insert";
                    out.insert(i+1, newMatch);
                }
                ++i;
            break;
        }
        if ( i == out.end() ) {
            out.push_back(*this);
            return out;
        }
    }
    return out;
}