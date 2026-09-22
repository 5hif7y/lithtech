
| Asset   | Filetype extension           | Nature                                           | Key Loader                 |
| ------- | ---------------------------- | ------------------------------------------------ | -------------------------- |
| Model   | .ltb (bin), .lta/.ltc (text) | Mesh + skeleton + anims + LODs                   | Model::Load / ltaModelLoad |
| Texture | .dtx                         | BPP_32 or S3TC DXT1/3/5, with mipmaps/cubemaps   | dtx_Create                 |
| Sprite  | .spr                         | List of frames pointing to .dtx                  | spr_Create                 |
| Sound   | .wav                         | RIFF: PCM / IMA ADPCM / MP3                      | GetWaveInfo + ACM          |
| World   | World.dat (bin), .lta (text) | BSP + sections (render, physics, light, objects) | CWorldSharedBSP::Load      |

[[Models .ltb, .lta, .ltc]]
[[Textures .dtx]]
[[Sprites .spr]]
[[Sounds .wav]]
[[World-Maps .dat, .lta]]


