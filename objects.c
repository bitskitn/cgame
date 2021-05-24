#include <stddef.h>

#include "objects.h"

Object *
createObject(Prototype prototype)
{
    return NULL;
}

void
destroyObject(Object *object)
{
    if (object == NULL)
        return;
    stopObserving(object->target, object);
    // Force all of observers of this object to stop observing it
    {
        for (Object *observer = object->firstObserverOfSelf;
             observer != NULL;
             observer = observer->nextObserverOfSameTarget)
        {
            observer->target = NULL;
        }
        object->firstObserverOfSelf = NULL;
    }
    // Remove the object from the list of objects with the same tag
    {
        if (object->previousObjectWithSameTag != NULL)
        {
            object->previousObjectWithSameTag->nextObjectWithSameTag =
                object->nextObjectWithSameTag;
        }
        if (object->nextObjectWithSameTag != NULL)
        {
            object->nextObjectWithSameTag->previousObjectWithSameTag =
                object->previousObjectWithSameTag;
        }
    }
}

void
forEachObjectWithTag(Tag tag, void (*applyTo)(Object *object))
{
    for (Object *object = NULL;
         object != NULL;
         object = object->nextObjectWithSameTag)
    {
        applyTo(object);
    }
}

void
forEachObjectIn(GroupMask groupMask, void (*applyTo)(Object *object))
{
    Object *object = NULL;
    if (object->group & groupMask)
        applyTo(object);
}

void
startObserving(Object *target, Object *observer)
{
    if (observer->target != target)
        stopObserving(observer->target, observer);
    if (target != NULL)
    {
        observer->target = target;
        observer->nextObserverOfSameTarget = target->firstObserverOfSelf;
        target->firstObserverOfSelf = observer;
    }
}

void
stopObserving(Object *target, Object *observer)
{
    if (target == NULL)
        target = observer->target;
    if (observer->target != target)
        return;
    // TODO: remove observer from the list of observers
    observer->target = NULL;
}
