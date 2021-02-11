#include "matchchunk.h"


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

QLinkedList<MatchChunk>& operator<<( QLinkedList<MatchChunk>& out, MatchChunk& object )
{
    QLinkedList<MatchChunk>::iterator i;

    if ( out.isEmpty() || object.startColumn() >= out.last().endColumn() ) {
        out.push_back(object);
        return out;
    }

    for (i = out.begin(); i != out.end();) {
        switch ( _Get_Position( *i, object) ) {
            case BEFORE:
                out.insert(i, object);
                return out;
            break;
            case AFTER:
                ++i;
            break;
            case BERORE_INTERSERT:
                i->setStartColumn(object.endColumn());
            break;
            case AFTER_INTERSERT:
                i->setEndColumn(object.startColumn());
                ++i;
            break;
            case CONTAIN:
                i = out.erase(i);
            break;
            case BE_CONTAIN:
                // 原本的MatchChunk被拆成了两半
                MatchChunk newMatch = MatchChunk ( object.endColumn(), i->endColumn(), i->type() );
                i->setEndColumn(object.startColumn());
                out.insert(i+1, newMatch);
                ++i;
            break;
        }
        if ( i == out.end() ) {
            out.push_back(object);
            return out;
        }
    }
    return out;
}