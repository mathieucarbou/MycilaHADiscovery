# Light

The `light` platform exposes lights with brightness, color temperature, one color style and effects. See the [Home Assistant light documentation](https://www.home-assistant.io/integrations/light.mqtt/).

The library implements the `basic` schema (which is Home Assistant's default and therefore omitted from payloads).

## Examples

=== "Dimmable white bulb"

    ```cpp
    auto bulb = std::make_unique<Mycila::HA::Light>("bulb", "Desk Bulb", "~/bulb/set");
    bulb->stateTopic = "~/bulb/state";
    bulb->brightnessCommandTopic = "~/bulb/bri/set";
    bulb->brightnessStateTopic = "~/bulb/bri";
    bulb->onCommandType = "brightness";
    haDiscovery.publish(std::move(bulb));
    ```

=== "RGB light with effects"

    ```cpp
    auto strip = std::make_unique<Mycila::HA::Light>("strip", "LED Strip", "~/strip/set");
    strip->stateTopic = "~/strip/state";
    strip->rgbCommandTopic = "~/strip/rgb/set";
    strip->rgbStateTopic = "~/strip/rgb";
    strip->colorTempCommandTopic = "~/strip/ct/set";
    strip->effectList = {"rainbow", "pulse"};
    strip->effectCommandTopic = "~/strip/fx/set";
    haDiscovery.publish(std::move(strip));
    ```

=== "Kelvin color temperature"

    ```cpp
    auto lamp = std::make_unique<Mycila::HA::Light>("lamp", "Kelvin Lamp", "~/lamp/set");
    lamp->colorTempKelvin = true; // payloads in kelvin instead of mireds
    lamp->minKelvin = 2700;
    lamp->maxKelvin = 6500;
    haDiscovery.publish(std::move(lamp));
    ```

## Configuration Variables

| Option | Field | Type | Required | Default |
| :-- | :-- | :-- | :------- | :------ |
| command_topic | `commandTopic` | string | **Required** | |
| state_topic | `stateTopic` | string | Optional | |
| state_value_template | `stateValueTemplate` | template | Optional | |
| payload_on | `payloadOn` | string | Optional | `ON` |
| payload_off | `payloadOff` | string | Optional | `OFF` |
| on_command_type | `onCommandType` | string | Optional | `last` (`last`, `first` or `brightness`) |
| brightness_command_topic | `brightnessCommandTopic` | string | Optional | |
| brightness_command_template | `brightnessCommandTemplate` | template | Optional | |
| brightness_state_topic | `brightnessStateTopic` | string | Optional | |
| brightness_value_template | `brightnessValueTemplate` | template | Optional | |
| brightness_scale | `brightnessScale` | `std::optional<int>` | Optional | `255` |
| color_temp_command_topic | `colorTempCommandTopic` | string | Optional | |
| color_temp_command_template | `colorTempCommandTemplate` | template | Optional | |
| color_temp_state_topic | `colorTempStateTopic` | string | Optional | |
| color_temp_value_template | `colorTempValueTemplate` | template | Optional | |
| color_temp_kelvin | `colorTempKelvin` | `std::optional<bool>` | Optional | `false` |
| min_kelvin | `minKelvin` | `std::optional<int>` | Optional | `2000` |
| max_kelvin | `maxKelvin` | `std::optional<int>` | Optional | `6535` |
| min_mireds | `minMireds` | `std::optional<int>` | Optional | |
| max_mireds | `maxMireds` | `std::optional<int>` | Optional | |
| hs_command_topic | `hsCommandTopic` | string | Optional | hue 0–360, saturation 0–100 |
| hs_command_template | `hsCommandTemplate` | template | Optional | |
| hs_state_topic | `hsStateTopic` | string | Optional | |
| hs_value_template | `hsValueTemplate` | template | Optional | |
| xy_command_topic | `xyCommandTopic` | string | Optional | CIE 1931 x,y |
| xy_command_template | `xyCommandTemplate` | template | Optional | |
| xy_state_topic | `xyStateTopic` | string | Optional | |
| xy_value_template | `xyValueTemplate` | template | Optional | |
| rgb_command_topic | `rgbCommandTopic` | string | Optional | r,g,b 0–255 |
| rgb_command_template | `rgbCommandTemplate` | template | Optional | |
| rgb_state_topic | `rgbStateTopic` | string | Optional | |
| rgb_value_template | `rgbValueTemplate` | template | Optional | |
| rgbw_command_topic | `rgbwCommandTopic` | string | Optional | r,g,b,w |
| rgbw_command_template | `rgbwCommandTemplate` | template | Optional | |
| rgbw_state_topic | `rgbwStateTopic` | string | Optional | |
| rgbw_value_template | `rgbwValueTemplate` | template | Optional | |
| rgbww_command_topic | `rgbwwCommandTopic` | string | Optional | r,g,b,ww,cw |
| rgbww_command_template | `rgbwwCommandTemplate` | template | Optional | |
| rgbww_state_topic | `rgbwwStateTopic` | string | Optional | |
| rgbww_value_template | `rgbwwValueTemplate` | template | Optional | |
| white_command_topic | `whiteCommandTopic` | string | Optional | |
| white_scale | `whiteScale` | `std::optional<int>` | Optional | `255` |
| color_mode_state_topic | `colorModeStateTopic` | string | Optional | |
| color_mode_value_template | `colorModeValueTemplate` | template | Optional | |
| effect_command_topic | `effectCommandTopic` | string | Optional | |
| effect_command_template | `effectCommandTemplate` | template | Optional | |
| effect_state_topic | `effectStateTopic` | string | Optional | |
| effect_value_template | `effectValueTemplate` | template | Optional | |
| effect_list | `effectList` | `std::vector<const char*>` | Optional | |

!!! note "Color styles"
    `hs`, `xy`, `rgb`, `rgbw` and `rgbww` are alternative color representations: configure the one matching your device. Set `color_mode_state_topic` only when needed for explicit color mode control.
