{
    "openapi":  "3.0.0",
    "info":
    {
        "title":        "LHModelDbUtilTest OAuth2.0 Spec",
        "description":  "LHModelDbUtilTest OAuth2.0 Spec Server Side Types",
        "version":      "1.0.0"
    },
    "servers":
    {
    },
    "paths":
    {
    },
    "components":
    {
        "schemas":
        {
            "ExampleModelA":
            {
                "type": "object",
                "properties":
                {
                    "pid":
                    {
                        "type": "string",
                        "format": "uuid"
                    },
                    "pit":
                    {
                        "type": "string",
                        "format": "date-time"
                    },
                    "token":
                    {
                        "type": "string"
                    },
                    "enabled":
                    {
                        "type": "boolean"
                    }
                }
            }
        }
    }
}
