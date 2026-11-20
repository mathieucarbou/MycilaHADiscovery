# Contributing

If the behavior of this library does not fit your needs, or you want to improve it, or add features: please make a pull request.

## Adding or changing a component

1. Check the component page on [home-assistant.io](https://www.home-assistant.io/integrations/mqtt/): the doc page is the source of truth for which options are `Required` vs `Optional`.
2. Add or update the component class in `src/MycilaHADiscovery.h`, following the existing conventions:
   - Required topics are constructor arguments; optional topics are `std::string` fields
   - Payloads and templates are borrowed `const char*`
   - Values whose omission lets Home Assistant apply its default are `std::optional` and are not emitted when unset
3. Validate every JSON key you emit against [Home Assistant's abbreviation list](https://www.home-assistant.io/integrations/mqtt/#supported-abbreviations-in-mqtt-discovery-messages).
4. Build an example with `pio run -e ci-arduino-3` and run `cpplint` as configured in the CI workflow.
5. Update the matching page under `docs/components/`.

## Documentation

The website is built with [mkdocs-material](https://squidfunk.github.io/mkdocs-material/) from this repository (`mkdocs.yml` + `docs/`), and published to GitHub Pages by the `website.yml` workflow. To preview locally:

```bash
pip install mkdocs-material
mkdocs serve
```
