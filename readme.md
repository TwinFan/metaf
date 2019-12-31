# Modern C++ library for parsing METAR and TAF with focus on Webassembly

[![pipeline status](https://gitlab.com/nnaumenko/metaf/badges/master/pipeline.svg)](https://gitlab.com/nnaumenko/metaf/commits/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/785c3536264b4a90bd8d94e1a799d275)](https://www.codacy.com/manual/nnaumenko/metaf?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=nnaumenko/metaf&amp;utm_campaign=Badge_Grade)
[![Codecov](https://codecov.io/gl/nnaumenko/metaf/branch/master/graph/badge.svg)](https://codecov.io/gl/nnaumenko/metaf)
[![MIT license](https://img.shields.io/github/license/nnaumenko/metaf)](LICENSE.md)
[![Try online](https://img.shields.io/badge/try-online-blue)](https://wandbox.org/permlink/NqVskzKVpZaQHasP)


## Introduction

Metaf is a header-only, dependency-free modern C++ library for parsing [METAR weather reports](https://en.wikipedia.org/wiki/METAR) and [TAF weather forecasts](https://en.wikipedia.org/wiki/Terminal_aerodrome_forecast) used in aviation.

[Live demo: Short weather summary for aerodromes near your location based on METAR/TAF weather data](https://nnaumenko.gitlab.io/metaf/examples/summary.html).

[Live demo: Decode METAR or TAF report and explain in English language](https://nnaumenko.gitlab.io/metaf/examples/explain.html).

This project focuses on using METAR and TAF parsing with Webassembly, however the library is has no dependencies and can be used in other environments.

Metaf can do the following:

* Parse METAR or TAF report and autodetect its type.
* Check the validity of the report syntax, detect malformed reports and report errors.
* Convert METAR or TAF report into the vector of classes which represent individual chunks of info encoded in the weather report or forecast.

## Documentation

Please refer to [documentation](https://nnaumenko.gitlab.io/metaf/docs/index.html) for details.

[Tutorial on basic usage of Metaf library](https://nnaumenko.gitlab.io/metaf/docs/getting_started.html).

## Compatible compilers

The following compiler are compatible (with C++17 standard enabled):

* emscripten emcc 1.38.28 (based on clang)
* gcc 7.4.0
* clang 8.0.0

The compatibility with the compilers above is routinely tested using Gitlab CI after each commit.

## Prerequisites and dependencies

Metaf requires C++17.

No external dependencies for parser itself, only standard C++ libraries used. 

Unit tests included with the project use [Google Test](https://github.com/abseil/googletest) framework.

Since this project focuses on metaf library usage with Webassembly, [Emscripten](emscripten.org) is required to build tests and examples. Running the tests and examples requires a www browser with Webassembly support.

## Limitations

Old TAF format, used before November 2008 employs different format for time spans and trends (time without date); the current version does not decode this old format.

## License

This project is available under MIT license.

## METAR and TAF

### What is METAR?

METAR is current weather report format used in aviation. Typical METAR report contains information such as location, report issue time, wind, visibility, clouds, weather phenomena, temperature, dewpoint and atmospheric pressure.

METAR in raw form is human-readable though it might look cryptic for untrained person.

Example of a simple METAR report is as follows: 

    METAR EICK 092100Z 23007KT 9999 FEW038 BKN180 11/08 Q1019 NOSIG=

### What is TAF?

TAF (Terminal Aerodrome Forecast) is a weather forecast report format used in aviation. TAF report is quite similar to METAR and reports trends and changes in visibility, wind, clouds, weather, etc over periods of time.

TAF in raw form is also human-readable but requires training to decode.

Example of a TAF report is as follows:

    TAF EICK 091700Z 0918/1018 27012KT 9999 BKN025 BECMG 0920/0923 24007KT BECMG 1002/1005 21007KT BECMG 1009/1012 21015KT TEMPO 1010/1013 -RA BKN012 TEMPO 1010/1018 21018G28KT BECMG 1013/1016 6000 -RA SCT003 BKN010 TEMPO 1014/1018 3000 -RADZ BKN003 PROB40 TEMPO 1015/1018 1200 BR BKN002=

### Which groups Metaf is able to recognize?

* Report type METAR, SPECI, and TAF
* Various amended or correctional report indicators
* Indicators for missing report, cancelled TAF report, and various indicator for missing data given in remarks section
* Automated report indicator, automated station type remark, and maintenance indicator
* ICAO location
* Report issue time
* Wind direction, speed and gust speed
* Wind shear, peak wind and wind shift information
* Prevailing or directional visibility (including variable visibility groups specified in remarks) in meters or statute miles
* Surface visibility and visibility from air traffic control tower.
* Cloud layer information, clear sky conditions, 'no significant cloud' / 'no cloud detected information', and detailed cloud layers information specified in remarks
* Cloud cover of variable density and variable ceiling height
* Indicator of no significant cloud and good visibility CAVOK
* Indicatiors for certain secondary locations (e.g. wind shear in the lower levels at path of runway approach, ceiling, etc), and indicators for missing visibility or ceiling data. 
* Current and recent weather information, and indicator or weather phenomena end NSW
* Beginning and ending time of recent weather phenomena
* Temperature and dew point, including more precise values given in remarks
* Temperature forecast from TAF reports
* 6-hourly and 24-hourly minimum and maximum temperature
* Current atmospheric pressure, including QNH, QFE, SLP remarks, atmospheric pressure tendency remark, and groups indicating rapid pressure rise or fall
* Forecast lowest atmospheric pressure
* Runway visual range with trend
* State of runway, type and amount of deposits, extent of runway contamination, surface friction and braking action
* Groups indicating that runway or airport is closed due to snow accumulation
* Groups indicating that deposits on runway were cleared or ceased to exist
* Rainfall groups used in Australia
* Various precipitation, snowfall, and ice buildup groups reported in remarks section in North America
* Temperature and state of sea surface or wave height
* Colour codes used by NATO militaries to quickly assess visibility and cloud conditions
* Trend groups NOSIG, BECMG, TEMPO, INTER, FMxxxxxx and various time span groups
* Indicator of no unscheduled reports being issued by station
* Groups indicating non-operational sensors
* Icing and turbulence forecast used by NATO militaries
* Recent precipitation beginning and ending time
* Lightning data such as strike type and frequency
* Groups reporting various phenomena in vicinity of the station (thunderstorm, towering cumulus, altocumulus castellanus, cumulonimbus, mammatus, lenticular and rotor clouds, virga, etc.)
* Sunshine duration
* Density altitude
* Largest hailstone size

## Roadmap

Recognition of the following groups is planned for implementing (not in this particular order):

* Additional visibility value for runway, e.g. VIS 2 1/2 RWY11
* Ground-based or aloft obscurations with octa, e.g. FG SCT000 or FU BKN020
* Recent precipitation group used in South America, e.g. PP000 

## Acknowledgements

This project uses [Emscripten](https://emscripten.org/) to compile examples into Webassembly.

Unit tests included with the project use [Google Test](https://github.com/abseil/googletest) framework.

[Gitlab CI/CD](https://docs.gitlab.com/ee/ci/) is used for deployment of the [project website](https://nnaumenko.gitlab.io/metaf/).

METAR, TAF, and weather station data used in [weather summary example](https://nnaumenko.gitlab.io/metaf/examples/summary.html) are from [Aviation Weather Center Text Data Server](https://www.aviationweather.gov/dataserver). 

Weather widget uses the following external content: [Weather Icons by Erik Flowers](http://weathericons.io) distributed under [SIL OFL 1.1 license](http://scripts.sil.org/OFL), [Source Sans Pro Font](https://fonts.google.com/specimen/Source+Sans+Pro) by Paul D. Hunt distributed under [Open Font License](http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&id=OFL_web), ['eye-regular'](https://fontawesome.com/icons/eye?style=regular) and ['icicles-solid'](https://fontawesome.com/icons/icicles?style=solid) icons by Font Awesome distributed under [Creative Commons Attribution 4.0 International license](https://fontawesome.com/license). METAR, TAF, and weather station data are acquired from [Aviation Weather Center Text Data Server](https://www.aviationweather.gov/dataserver) using [CORS-anywhere](https://cors-anywhere.herokuapp.com/).

The project repository is hosted on [Gitlab](https://about.gitlab.com/) and mirrored to [Github](https://github.com/).