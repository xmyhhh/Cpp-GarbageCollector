#include <cassert>
#include <cstdlib>

typedef enum
{
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

typedef struct sObject
{
    unsigned char marked;
    ObjectType type;
    struct sObject* next; //for track
    union
    {
        /* OBJ_INT */
        int value;

        /* OBJ_PAIR */
        struct
        {
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;

#define STACK_MAX 256

typedef struct
{
    Object* firstObject; //for track
    Object* stack[STACK_MAX];
    int stackSize;
} VM;

////VM
VM* newVM()
{
    VM* vm = static_cast<VM*>(malloc(sizeof(VM)));
    vm->stackSize = 0;
    return vm;
}

void push(VM* vm, Object* value)
{
    assert(vm->stackSize < STACK_MAX, "Stack overflow!");
    vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm)
{
    assert(vm->stackSize > 0, "Stack underflow!");
    return vm->stack[--vm->stackSize];
}

////Obj
Object* newObject(VM* vm, ObjectType type)
{
    Object* object = static_cast<Object*>(malloc(sizeof(Object)));
    object->type = type;
    object->marked = 0;

    //for track
    object->next = vm->firstObject;
    vm->firstObject = object;
    return object;
}

void pushInt(VM* vm, int intValue)// did not used 
{
    Object* object = newObject(vm, OBJ_INT);
    object->value = intValue;
    push(vm, object);
}

Object* pushPair(VM* vm)// did not used 
{
    Object* object = newObject(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);

    push(vm, object);
    return object;
}
//mark
void mark(Object* object)
{
    if (object->marked) return;

    object->marked = 1;

    if (object->type == OBJ_PAIR)
    {
        mark(object->head);
        mark(object->tail);
    }
}

void markAll(VM* vm)
{
    for (int i = 0; i < vm->stackSize; i++)
    {
        mark(vm->stack[i]);
    }
}
//sweep
void sweep(VM* vm)
{
    Object** object = &vm->firstObject;
    while (*object) {
        if (!(*object)->marked) {
            /* This object wasn't reached, so remove it from the list
               and free it. */
            Object* unreached = *object;

            *object = unreached->next;
            free(unreached);
        } else {
            /* This object was reached, so unmark it (for the next GC)
               and move on to the next. */
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

//GC
void gc(VM* vm) {
    markAll(vm);
    sweep(vm);
}

int main()
{
    return 0;
}
