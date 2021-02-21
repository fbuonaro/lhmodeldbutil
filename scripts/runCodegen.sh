docker run -v ${PWD}/liblhmodeldbutil/resource/swagger:/lhmodeldbutil --rm lhmodel-scg:main \
  -i /lhmodeldbutil/lhmodeldbutil_test.swagger.js \
  -c /lhmodeldbutil/lhmodeldbutil_test.config.js \
  --additional-properties \
    namespace=LHModelDbUtilTestNS,\
    lcModelFilename=true,\
    customFormatTypesFile=/lhmodeldbutil/customTypes.js \
  -o /lhmodeldbutil
