if not exist %PluginDepsBasePath% (
    curl -o %PluginDepsBasePath%.zip -kLO https://github.com/ThorAsgardDev/obs-yu-gi-oh-plugin-deps/archive/1.0.0.zip -f --retry 5 -C -
    7z x %PluginDepsBasePath%.zip -o%PluginDepsBasePath%
) else (
    echo "OBS dependencies are already there. Download skipped."
)
