# TraceDump

This project is second develop base on [TraceUtility](https://github.com/Qusic/TraceUtility)
[Blog post](https://sherlockz.github.io/2018/02/14/traceDump/)(in Chinese 中文)
## Usage
    $./traceDump path/to/xxx.trace
## Output
The same as TraceUtility
## Done
- Rebuild project and fix some bugs
- Support `GPU Driver log`instrument.
- Support `CPU Activity log`instrument.
- Support `Activity Monitor`instrument.
- Update `Allocations`instrument.(*Now you can get persisentBytes size during each second*)

## TODO
- Parse `Time Profiler`instrument.(*Old way dosen't work from Xcode 9.0*)
