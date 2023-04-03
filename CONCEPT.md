# KnxrCfg
Just another key -> value config schema


This is a mix out of the default Windows config "ini" and a json yaml variation

## basic structure (not concrete yet):
// file extension should something like this be: .knxrcfg .kxcfg or with a config name
// the config handler will search for these terms in the . path except you give a name, then it will just chill
// we have a priority system, the first .knxrcfg or .kxcfg file that is found, will be used

(Some wild category name)
// Some types are supported some are not, gl finding out what are. Jk strings, int, map/dict, vector are supported.
// but inline aren't, not yet
token="Some random Tokens"

coolVector=["a vector can have multiple types"]
// Hey it's called steve
coolObject={giveItAName: "Steve"} // vectors are later in the game
