project('domanek_project_1', 'cpp',
        license : ['Boost'],
        version : '1',
        default_options : ['cpp_std=c++17', 'warning_level=3', 'werror=true'])

thread_dep = dependency('threads')

# CLI11: https://github.com/CLIUtils/CLI11
add_global_arguments('-I' + get_option('cli11_include_dir'), language : 'cpp')

# spdlog: https://github.com/gabime/spdlog
add_global_arguments('-I' + get_option('spdlog_include_dir'), language : 'cpp')

# json: https://github.com/nlohmann/json
add_global_arguments('-I' + get_option('json_include_dir'), language : 'cpp')

# tabulate: https://github.com/p-ranav/tabulate
add_global_arguments('-I' + get_option('tabulate_include_dir'), language : 'cpp')

# toml++: https://github.com/marzer/tomlplusplus/
add_global_arguments('-I' + get_option('toml_include_dir'), language : 'cpp')

inc_dir = include_directories('include')
src = ['src/main.cpp']

executable('leb128',
           sources : src,
           include_directories : inc_dir,
           dependencies : [thread_dep]
          )
