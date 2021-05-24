#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdint.h>

typedef struct Object Object;

typedef uint16_t Prototype;
typedef uint8_t Tag;
typedef uint8_t Group;

typedef uint32_t GroupMask;

struct Object
{
    Prototype prototype;
    Tag tag;
    Group group;
    Object *previousObjectWithSameTag,
           *nextObjectWithSameTag;
    Object *firstObserverOfSelf;
    Object *nextObserverOfSameTarget,
           *target;
};

enum
{
    ALL_GROUPS = ~0
};

Object *createObject(Prototype prototype);
void destroyObject(Object *object);

void forEachObjectWithTag(Tag tag, void (*action)(Object *object));
void forEachObjectIn(GroupMask groupMask, void (*action)(Object *object));

void startObserving(Object *target, Object *observer);
void stopObserving(Object *target, Object *observer);

#endif