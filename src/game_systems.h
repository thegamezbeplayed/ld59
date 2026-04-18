#ifndef __GAME_SYS__
#define __GAME_SYS__


typedef void (*EventCallback)(
    EventType  event,
    void*      event_data,
    void*      user_data
);

typedef struct {
    EventType      event;
    EventCallback  cb;
    uint64_t       uid;
    void*          user_data;
} event_sub_t;

typedef struct {
    event_sub_t* subs;
    int count, cap;
} event_bus_t;

typedef struct{
  EventType   type;
  EventStatus status;
  int         max, calls;
  void*       data;
  uint64_t    iuid;
}event_t;

event_bus_t* InitEventBus(int cap);
event_sub_t* EventSubscribe(event_bus_t* bus, EventType event, EventCallback cb, void* u_data);
void EventEmit(event_bus_t* bus, event_t*);
void EventRemove(event_bus_t* bus, uint64_t id);

#endif
