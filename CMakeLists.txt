cmake_minimum_required(VERSION 3.22)
project(HandpanDesktopApp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.5
)
FetchContent_MakeAvailable(juce)

juce_add_gui_app(HandpanDesktopApp
  PRODUCT_NAME "Handpan"
)

target_sources(HandpanDesktopApp PRIVATE
  Source/main.cpp
  Source/App/MainComponent.cpp
  Source/Audio/HandpanEngine.cpp
  Source/Audio/HandpanVoice.cpp
  Source/Samples/SampleManager.cpp
  Source/Mapping/NoteMapping.cpp
  Source/Presets/ScalePresetManager.cpp
  Source/UI/HandpanPadComponent.cpp
)

target_include_directories(HandpanDesktopApp PRIVATE Source)

target_compile_definitions(HandpanDesktopApp PRIVATE
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:HandpanDesktopApp,JUCE_PRODUCT_NAME>"
  JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:HandpanDesktopApp,JUCE_VERSION>"
)

target_link_libraries(HandpanDesktopApp PRIVATE
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_gui_extra
)

juce_generate_juce_header(HandpanDesktopApp)
