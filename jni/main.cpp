#include <And64InlineHook/And64InlineHook.hpp>
#include <dlfcn.h>
#include <thread>
bool (*old_UNGISetInappState)(...);
bool UNGISetInappState(void *instance, bool a){
    return old_UNGISetInappState(instance, true);
}
bool (*old_AHNRGMSetInappState)(...);
bool AHNRGMSetInappState(void *instance, bool a){
    return old_AHNRGMSetInappState(instance, true);
}
bool (*old_URDSetInappState)(...);
bool URDSetInappState(void *instance, bool a){
    return old_URDSetInappState(instance, true);
}
bool SaveSystemGet(void *instance){
	return true;
}
void main_geokar2006_thread(){
	void *lib_data = nullptr;
    do {
        lib_data = dlopen("libUE4.so", RTLD_LAZY);
    } while (!lib_data);
	void *tmp;
	/// Ptrs
	void *URD_SET = dlsym(lib_data, "_ZN13URecoveryData13SetInappStateEb");
	void *AHNRGM_SET = dlsym(lib_data, "_ZN28AHelloNeighborRebornGameMode13SetInappStateEb");
	void *UNGI_SET = dlsym(lib_data, "_ZN26UHelloNeighborGameInstance13SetInappStateEb");
	void *SAVESYSTEM_GET = dlsym(lib_data, "_ZN11USaveSystem17GetInAppStateExecEv");
	
	/// Hooks
	if (URD_SET)
		A64HookFunction(URD_SET, (void *)URDSetInappState, (void **)&old_URDSetInappState);
    if (AHNRGM_SET)
		A64HookFunction(AHNRGM_SET, (void *)AHNRGMSetInappState, (void **)&old_AHNRGMSetInappState);
	if (SAVESYSTEM_GET)
		A64HookFunction(SAVESYSTEM_GET, (void *)SaveSystemGet, (void **)&tmp);
	if (UNGI_SET)
		A64HookFunction(UNGI_SET, (void *)UNGISetInappState, (void **)&old_UNGISetInappState);
}
__attribute__((constructor))
void lib_start() {
	std::thread(main_geokar2006_thread).detach();
}
// Garbage required by the compiler.
int main() {
	return 1;
}