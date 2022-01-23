find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
  file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*")
  file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*")
  file(GLOB_RECURSE GENERATED_HEADERS CONFIGURE_DEPENDS
       "${PROJECT_BINARY_DIR}/include/*")
  file(GLOB_RECURSE DOCUMENTATION CONFIGURE_DEPENDS
       "${CMAKE_CURRENT_SOURCE_DIR}/*.md")

  set(SPECIAL_DOCS
      "${PROJECT_SOURCE_DIR}/README.md" "${PROJECT_SOURCE_DIR}/LICENSE.md"
      "${PROJECT_SOURCE_DIR}/CHANGELOG.md")

  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
  set(DOXYGEN_PROJECT_NUMBER "v${PROJECT_VERSION}")
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_HTML_EXTRA_STYLESHEET
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/doxygen-awesome.css"
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/doxygen-awesome-sidebar-only.css"
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/doxygen-awesome-sidebar-only-darkmode-toggle.css"
  )
  set(DOXYGEN_HTML_EXTRA_FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/assets/doxygen-awesome-darkmode-toggle.js")
  set(DOXYGEN_HTML_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/assets/header.html")
  set(DOXYGEN_HTML_FOOTER "${CMAKE_CURRENT_SOURCE_DIR}/assets/footer.html")
  set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
  set(DOXYGEN_DOT_TRANSPARENT YES)

  doxygen_add_docs(
    docs
    ${SPECIAL_DOCS}
    ${HEADERS}
    ${SOURCES}
    ${GENERATED_HEADERS}
    ${DOCUMENTATION}
    USE_STAMP_FILE)
endif()