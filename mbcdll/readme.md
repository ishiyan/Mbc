# Examples

```c#
public class SampleClass : IDisposable
{    
    [DllImport("YourDll.dll", EntryPoint="ConstructorOfYourClass", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.ThisCall)]
    public extern static void SampleClassConstructor(IntPtr thisObject);

    [DllImport("YourDll.dll", EntryPoint="DestructorOfYourClass", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.ThisCall)]
    public extern static void SampleClassDestructor(IntPtr thisObject);

    [DllImport("YourDll.dll", EntryPoint="DoSomething", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.ThisCall)]
    public extern static void DoSomething(IntPtr thisObject);

    [DllImport("YourDll.dll", EntryPoint="DoSomethingElse", CharSet=CharSet.Ansi, CallingConvention=CallingConvention.ThisCall)]
    public extern static void DoSomething(IntPtr thisObject, int x);

    IntPtr ptr;

    public SampleClass(int sizeOfYourCppClass)
    {
        this.ptr = Marshal.AllocHGlobal(sizeOfYourCppClass);
        SampleClassConstructor(this.ptr);  
    }

    public void DoSomething()
    {
        DoSomething(this.ptr);
    }

    public void DoSomethingElse(int x)
    {
        DoSomethingElse(this.ptr, x);
    }

    public void Dispose()
    {
        if (this.ptr != IntPtr.Zero)
        {
            // The following 2 calls equals to "delete object" in C++
            // Calling the destructor of the C++ class will free the memory allocated by the native c++ class.
            SampleClassDestructor(this.ptr);

            // Free the memory allocated from .NET.
            Marshal.FreeHGlobal(this.ptr);

            this.ptr = IntPtr.Zero;
        }
    }
}
```

## Returning a char*

```c++
extern char * __stdcall GetVersionCharPtr()
{
    return "Version 3.1.2";
}
```

```c#
static class ImportLibrary
{
    const String DLL_LOCATION = "DemoLibrary.dll";

    [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern IntPtr GetVersionCharPtr();
}

IntPtr intPtr = ImportLibrary.GetVersionCharPtr();
string version = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(intPtr);
```

## Passing a string as a char* parameter
```c++
extern void __stdcall SetVersionCharPtr(char *version)
{
    // Do something here .. 
}
```

```c#
static class ImportLibrary 
{ 
    const String DLL_LOCATION = "DemoLibrary.dll"; 

    [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)] 
    public static extern void SetVersionCharPtr([MarshalAs(UnmanagedType.LPStr) string version); 
} 

ImportLibrary.SetVersionCharPtr("Version 1.0.0);
```

## Returning a string as a char** parameter

```c++
extern HRESULT __stdcall GetVersionCharPtrPtr(char **version)
{
    // a danger of memory leaks if the unmanaged DLL allocates the memory for the string on the heap
    *version = "Version 1.0.0"; 
    return S_OK;
}
```

```c#
static class ImportLibrary
{
    const String DLL_LOCATION = "DemoLibrary.dll"; 

    [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
    public static extern void GetVersionCharPtrPtr(out IntPtr version);
}

IntPtr intPtr;
ImportLibrary.GetVersionCharPtrPtr(out intPtr);
string version = System.Runtime.InteropServices.Marshal.PtrToStringAnsi(intPtr);
```

## Returning a string with a buffer

```c++
extern void __stdcall GetVersionBuffer(char *buffer, unsigned long *pSize)
{
    if (pSize == nullptr)
    {
        return;
    }

    static char *version = "Version 5.1.1";
    unsigned long size = strlen(version) + 1;
    if ((buffer != nullptr) && (*pSize >= size))
    {
        strcpy_s(buffer, size, s_lastSetVersion);
    }
    // The string length including the zero terminator
    *pSize = size;
}
```

```c#
static class ImportLibrary
{
    const String DLL_LOCATION = "DemoLibrary.dll"; 

    [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi)]
    public static extern Boolean GetVersionBuffer([MarshalAs(UnmanagedType.LPStr)] StringBuilder version, ref UInt32 size);
}

UInt32 size = 0;
Model.ImportLibrary.GetVersionBuffer(null, ref size);
            
var sb = new StringBuilder((int)size);
ImportLibrary.GetVersionBuffer(sb, ref size);
string version = sb.ToString();
```
