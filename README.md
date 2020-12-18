# LibKWeather

## Introduction
Get weather forecast and alerts anywhere on the earth without worrying about low level thing.
LibKWeather provides you a highly abstracted library for anything related to weather.

Features:
* Get local weather forecast.
* Get weather of a location by name or coordinate.
* Get sunrise/set moonrise/set and many more informations about a location
* Weather alerts of a country (ToDo)

### Usage Example
    The following snippet will get the weather forecast of Paris, France.
\snippet snippets/example.cpp example

### Classes
* [WeatherForecastSource](\ref WeatherForecastSource) Request weatherforecast by coordinates, everything will be fetched if not provided
* [PendingWeatherForecast](\ref PendingWeatherForecast) Pending object for asynchronous data fetching
* [SunriseSource](\ref SunriseSource) Fetch sunrise informations, used internally by [WeatherForecastSource](\ref WeatherForecastSource)
* [LocationQuery](\ref LocationQuery) Query places by name or locate the current position
* [GeoTimezone](\ref GeoTimezone) Class to obtain timezone via coordinate

## Links

- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: #kde-devel on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/libkweather/repository>
