find_package(Doxygen REQUIRED)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/specula/**/*")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*")
file(GLOB_RECURSE DOCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.md")
list(APPEND DOCS "${PROJECT_SOURCE_DIR}/README.md" "${PROJECT_SOURCE_DIR}/LICENSE.md"
      "${PROJECT_SOURCE_DIR}/CHANGELOG.md"
)

set(DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}")
set(DOXYGEN_STRIP_FROM_INC "${PROJECT_SOURCE_DIR}/include/specula")
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
set(DOXYGEN_PROJECT_NUMBER "v${PROJECT_VERSION}")
set(DOXYGEN_GENERATE_TREEVIEW YES)

set(DOXYGEN_HTML_EXTRA_STYLESHEET
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/css/doxygen-awesome.css"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/css/doxygen-awesome-sidebar-only.css"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/css/doxygen-awesome-sidebar-only-darkmode-toggle.css"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/css/override.css"
)
set(DOXYGEN_HTML_EXTRA_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/js/doxygen-awesome-darkmode-toggle.js"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/js/doxygen-awesome-fragment-copy-button.js"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/js/doxygen-awesome-paragraph-link.js"
)

set(DOXYGEN_LAYOUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/assets/DoxygenLayout.xml")
set(DOXYGEN_HTML_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/assets/html/header.html")
set(DOXYGEN_HTML_FOOTER "${CMAKE_CURRENT_SOURCE_DIR}/assets/html/footer.html")

set(DOXYGEN_FILE_VERSION_FILTER "git log -n 1 --pretty=format:%h --")
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_DOT_IMAGE_FORMAT svg)
set(DOXYGEN_DOT_FONTNAME "Iosevka")
set(DOXYGEN_DOT_TRANSPARENT YES)
set(DOXYGEN_HTML_COLORSTYLE_HUE 209)
set(DOXYGEN_HTML_COLORSTYLE_SAT 255)
set(DOXYGEN_HTML_COLORSTYLE_GAMMA 113)

doxygen_add_docs(docs ${HEADERS} ${SOURCES} ${DOCS})