#ifndef MATCHCHUNK_H
#define MATCHCHUNK_H

#include <QLinkedList>
#include <QtGlobal>
#include <QColor> 

// Represents a match result for Filter and QuickFind
class MatchChunk
{
  public:
    enum MatchChunkType {
        Filter,
        QuickFind,
    };
    // Construct a match (must be initialised)
    MatchChunk( int start_column, int length , MatchChunkType type)
    { 
        type_ = type;
        startColumn_ = start_column;
        length_ = length; 
        endColumn_ = start_column + length;
    }

    // Accessor functions
    MatchChunkType type() const { return type_; }
    int startColumn() const { return startColumn_; }
    int endColumn() const { return endColumn_; }
    int length() const { return length_; }
    const QColor& foreColor() const { return foreColor_; }
    const QColor& backColor() const { return backColor_; }

    // Set functions
    void setStartColumn( int value ) {
        startColumn_ = value;
        length_ = endColumn_ - startColumn_;
    }
    void setEndColumn( int value ) {
        endColumn_ = value;
        length_ = endColumn_ - startColumn_;
    }
    void setColor(QColor& foreColor, QColor& backColor) 
    {
        foreColor_ = foreColor;
        backColor_ = backColor;
    }

  private:
    MatchChunkType type_;
    int startColumn_;
    int length_;
    int endColumn_;
    QColor foreColor_;
    QColor backColor_;
};

enum Relative_Position {
    BEFORE = 0,
    BERORE_INTERSERT,
    AFTER_INTERSERT,
    AFTER,
    BE_CONTAIN,
    CONTAIN,
};

QLinkedList<MatchChunk>& operator<<( QLinkedList<MatchChunk>& out, MatchChunk& object );

#endif