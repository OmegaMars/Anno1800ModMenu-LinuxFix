# InjectionScheduler

## Purpose

Starts menu DLL injection once, after `DllMain` has returned.

Wine/Proton often crashes if `CreateThread` runs inside `DllMain` (loader lock).
This class is triggered from the first forwarded `version.dll` export instead.
