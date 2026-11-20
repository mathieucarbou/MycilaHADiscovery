# Lock

The `lock` platform exposes door locks, garages... with lock, unlock and optional open commands. See the [Home Assistant lock documentation](https://www.home-assistant.io/integrations/lock.mqtt/).

```cpp
auto lock = std::make_unique<Mycila::HA::Lock>("door", "Front Door", "~/door/lock/set");
lock->stateTopic = "~/door/lock/state";
lock->payloadOpen = "OPEN";
lock->stateJammed = "JAMMED";
lock->codeFormat = "^\\d{4,6}$";
haDiscovery.publish(std::move(lock));
```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | Optional | |
| value_template | `valueTemplate` | template | Optional | |
| command_template | `commandTemplate` | template | Optional | |
| code_format | `codeFormat` | string (regex) | Optional | |
| payload_lock | `payloadLock` | string | Optional | `LOCK` |
| payload_unlock | `payloadUnlock` | string | Optional | `UNLOCK` |
| payload_open | `payloadOpen` | string | Optional | unset = no `lock.open` action |
| payload_reset | `payloadReset` | string | Optional | `None` |
| state_locked | `stateLocked` | string | Optional | `LOCKED` |
| state_locking | `stateLocking` | string | Optional | `LOCKING` |
| state_unlocked | `stateUnlocked` | string | Optional | `UNLOCKED` |
| state_unlocking | `stateUnlocking` | string | Optional | `UNLOCKING` |
| state_jammed | `stateJammed` | string | Optional | `JAMMED` |

!!! warning "Retain state messages"
    Keep the retain flag consistent for `state_topic`: an unlocked state replayed at startup must match the physical lock state. Home Assistant warns about retained messages on lock entities for this reason.
