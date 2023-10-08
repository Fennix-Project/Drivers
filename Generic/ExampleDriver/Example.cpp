#include "../../../Kernel/mapi.hpp"
#include "../../../Kernel/Fex.hpp"

extern "C" int ModuleEntry(void *Data);
int CallbackHandler(KernelCallback *Data);
int InterruptCallback(CPURegisters *Registers);

/*                           The module is
 *       This is a module     for Fennix     Module Entry Extended Header
 *              *                   *              *             */
HEAD(FexFormatType_Module, FexOSType_Fennix, ModuleEntry);

/* Ignore the warning about missing field initializers. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

/* Extended header which is used to give additional information to the kernel. */
__attribute__((section(".extended"))) FexExtended ExtendedHeader = {
	.Module = {
		.Name = "Example Module",
		.Type = FexModuleType_Generic,
		.Callback = CallbackHandler,
		.InterruptCallback = InterruptCallback,
		.Bind = {
			.Type = BIND_INTERRUPT,
			.Interrupt = {
				.Vector = {222}, /* IRQ222 */
			}}}};

#pragma GCC diagnostic pop

/* Global structure that holds the kernel API. */
KernelAPI KAPI{};

/* Macro that prints a message to the kernel debugger. */
#define print(msg) KAPI.Util.DebugPrint((char *)(msg), KAPI.Info.modUniqueID)

/* --------------------------------------------------------------------------------------------------------- */

/* Module entry point. This is called at initialization.
   "Data" argument points to the kernel API structure. */
int ModuleEntry(void *Data)
{
	/* Check if kernel API is valid */
	if (!Data)
		return INVALID_KERNEL_API;

	/* Set the global structure to the kernel API. */
	KAPI = *(KernelAPI *)Data;

	/* Check if kernel API version is valid.
	   This is important because the kernel
	   API may change in the future. */
	if (KAPI.Version.Major < 0 || KAPI.Version.Minor < 0 || KAPI.Version.Patch < 0)
		return KERNEL_API_VERSION_NOT_SUPPORTED;

	/* We print "Hello World!" to the kernel debugger. */
	print("Hello World!");
	return OK;
}

/* This is called when the module is bound to an interrupt, process,
   or PCI device or when the kernel wants to send a message to the module. */
int CallbackHandler(KernelCallback *Data)
{
	switch (Data->Reason)
	{
	case AcknowledgeReason:
	{
		print("Kernel acknowledged the module.");
		break;
	}
	case StopReason:
	{
		print("Module stopped.");
		break;
	}
	default:
	{
		print("Unknown reason.");
		break;
	}
	}
	return OK;
}

/* Interrupt handler. */
int InterruptCallback(CPURegisters *)
{
	print("Interrupt received.");
	return OK;
}
