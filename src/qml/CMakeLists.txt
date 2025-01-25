# SPDX-FileCopyrightText: 2025 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

ecm_add_qml_module(kweathercoreqmlplugin
    URI "org.kde.weathercore"
    VERSION 1.0
    GENERATE_PLUGIN_SOURCE
)

target_sources(kweathercoreqmlplugin PRIVATE
    types.h
)

target_link_libraries(kweathercoreqmlplugin PRIVATE Qt::Qml KWeatherCore)

ecm_finalize_qml_module(kweathercoreqmlplugin DESTINATION ${KDE_INSTALL_QMLDIR})
