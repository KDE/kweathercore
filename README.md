# KWeatherCore

## Introduction
Get weather forecast and alerts anywhere on the earth easy.
KWeatherCore provides you a highly abstracted library for things related to weather.

Features:
* Get local weather forecast.
* Get weather of a location by name or coordinate.
* Weather alerts of a country (ToDo)

### Usage Example
    The following snippet will get the weather forecast of Paris, France.
\snippet snippets/example.cpp example

### Classes
* [WeatherForecastSource](\ref WeatherForecastSource) Request weatherforecast by coordinates, everything will be fetched if not provided
* [PendingWeatherForecast](\ref PendingWeatherForecast) Pending object for asynchronous data fetching
* [LocationQuery](\ref LocationQuery) Query places by name or locate the current position
* [GeoTimezone](\ref GeoTimezone) Class to obtain timezone via coordinate

## Services Used
* [Norwegian Meteorological Institute](https://www.met.no/) WeatherForecast
* [Geonames.org](https://geonames.org) Location name search

## Links

- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-devel>
- IRC channel: #kde-devel on Libera Chat
- Git repository: <https://invent.kde.org/libraries/kweathercore/>
