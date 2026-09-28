{
    "Source": "GodRays.azsl",
    "DepthStencilState": {
        "Depth": {
            "Enable": false
        }
    },
    "ProgramSettings": {
        "EntryPoints": [
            {
                "name": "MainVS",
                "type": "Vertex"
            },
            {
                "name": "MainPS",
                "type": "Fragment"
            }
        ]
    },
    "GlobalTargetBlendState": {
        "Enable": true,
        "BlendSource": "One",
        "BlendDest": "One",
        "BlendOp": "Add",
        "BlendAlphaSource": "Zero",
        "BlendAlphaDest": "One",
        "BlendAlphaOp": "Add"
    }
}
