# FAQ (EN)

## Are these real kernel extensions?

Yes. Current binaries are `MH_KEXT_BUNDLE` and use `CFBundlePackageType = KEXT`.

## Can I use them in EFI (OpenCore)?

Yes. This is the recommended Hackintosh flow.

## Can I install them in /Library/Extensions?

Yes, but this is a legacy/advanced path and depends on macOS security policy.

## Which architecture is supported in this repo?

`x86_64` in the default build pipeline.

## Are the kexts signed with an Apple production kext certificate?

Not in the default local flow. The pipeline uses ad-hoc signing for testing.

## How do I rebuild the binaries in the `kexts` directory?

From repository root:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
```
