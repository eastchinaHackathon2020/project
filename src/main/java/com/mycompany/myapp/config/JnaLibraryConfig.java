package com.mycompany.myapp.config;

import com.sun.jna.*;

import java.awt.*;
import java.io.*;
import java.lang.ref.WeakReference;
import java.lang.reflect.*;
import java.net.URL;
import java.net.URLDecoder;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.util.Collections;
import java.util.Map;
import java.util.WeakHashMap;

public class JnaLibraryConfig {

    private static Map typeMappers = new WeakHashMap();
    private static Map alignments = new WeakHashMap();
    private static Map options = new WeakHashMap();
    private static Map libraries = new WeakHashMap();


    public static final int POINTER_SIZE;
    public static final int LONG_SIZE;
    public static final int WCHAR_SIZE;
    private static final int UNSPECIFIED = -1;
    private static final int MAC = 0;
    private static final int LINUX = 1;
    private static final int WINDOWS = 2;
    private static final int SOLARIS = 3;
    private static final int FREEBSD = 4;
    private static final int osType;

    static {
        String osName = System.getProperty("os.name");
        if (osName.startsWith("Linux")) {
            osType = LINUX;
        }
        else if (osName.startsWith("Mac") || osName.startsWith("Darwin")) {
            osType = MAC;
        }
        else if (osName.startsWith("Windows")) {
            osType = WINDOWS;
        }
        else if (osName.startsWith("Solaris") || osName.startsWith("SunOS")) {
            osType = SOLARIS;
        }
        else if (osName.startsWith("FreeBSD")) {
            osType = FREEBSD;
        }
        else {
            osType = UNSPECIFIED;
        }
    }
    public static final boolean isMac() {
        return osType == MAC;
    }
    public static final boolean isLinux() {
        return osType == LINUX;
    }
    public static final boolean isWindows() {
        return osType == WINDOWS;
    }
    public static final boolean isSolaris() {
        return osType == SOLARIS;
    }
    public static final boolean isFreeBSD() {
        return osType == FREEBSD;
    }
    public static final boolean isX11() {
        // TODO: check FS or do some other X11-specific test
        return !Platform.isWindows() && !Platform.isMac();
    }
    static {
        try {
            System.loadLibrary("jnidispatch");
        }
        catch(UnsatisfiedLinkError e) {
            loadNativeLibrary();
        }
        POINTER_SIZE = pointerSize();
        LONG_SIZE = longSize();
        WCHAR_SIZE = wideCharSize();
        initIDs();
        if (Boolean.getBoolean("jna.protected")) {
            setProtected(true);
        }
    }

    private JnaLibraryConfig() { }

    private static native void initIDs();


    public static synchronized native void setProtected(boolean enable);


    public static synchronized native boolean isProtected();

    /** Set whether the system last error result is captured after every
     * native invocation.  Defaults to <code>true</code>.
     */
    public static synchronized native void setPreserveLastError(boolean enable);

    /** Indicates whether the system last error result is preserved
     * after every invocation.
     */
    public static synchronized native boolean getPreserveLastError();

    /** Utility method to get the native window ID for a Java {@link Window}
     * as a <code>long</code> value.
     * This method is primarily for X11-based systems, which use an opaque
     * <code>XID</code> (usually <code>long int</code>) to identify windows.
     * @throws HeadlessException if the current VM is running headless
     */
    public static long getWindowID(Window w) throws HeadlessException {
        return getComponentID(w);
    }

    /** Utility method to get the native window ID for a heavyweight Java
     * {@link Component} as a <code>long</code> value.
     * This method is primarily for X11-based systems, which use an opaque
     * <code>XID</code> (usually <code>long int</code>) to identify windows.
     * @throws HeadlessException if the current VM is running headless
     */
    public static long getComponentID(Component c) throws HeadlessException {
        if (GraphicsEnvironment.isHeadless()) {
            throw new HeadlessException("No native windows when headless");
        }
        if (c.isLightweight()) {
            throw new IllegalArgumentException("Component must be heavyweight");
        }
        if (!c.isDisplayable())
            throw new IllegalStateException("Component must be displayable");
        // On X11 VMs prior to 1.5, the window must be visible
        if (Platform.isX11()
            && System.getProperty("java.version").matches("^1\\.4\\..*")) {
            if (!c.isVisible()) {
                throw new IllegalStateException("Component must be visible");
            }
        }
        // By this point, we're certain that Toolkit.loadLibraries() has
        // been called, thus avoiding AWT/JAWT link errors
        // (see http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6539705).
        return getWindowHandle0(c);
    }

    /** Utility method to get the native window pointer for a Java
     * {@link Window} as a {@link Pointer} value.  This method is primarily for
     * w32, which uses the <code>HANDLE</code> type (actually
     * <code>void *</code>) to identify windows.
     * @throws HeadlessException if the current VM is running headless
     */

    /** Utility method to get the native window pointer for a heavyweight Java
     * {@link Component} as a {@link Pointer} value.  This method is primarily
     * for w32, which uses the <code>HANDLE</code> type (actually
     * <code>void *</code>) to identify windows.
     * @throws HeadlessException if the current VM is running headless
     */

    private static native long getWindowHandle0(Component c);

    /** Convert a direct {@link Buffer} into a {@link Pointer}.
     * @throws IllegalArgumentException if the buffer is not direct.
     * @deprecated Use {@link #getDirectBufferPointer} instead.
     */
    public static Pointer getByteBufferPointer(ByteBuffer b) {
        return getDirectBufferPointer(b);
    }

    /** Convert a direct {@link Buffer} into a {@link Pointer}.
     * @throws IllegalArgumentException if the buffer is not direct.
     */
    public static native Pointer getDirectBufferPointer(Buffer b);

    /** Obtain a Java String from the given native byte array.  If there is
     * no NUL terminator, the String will comprise the entire array.  If the
     * system property <code>jna.encoding</code> is set, its value will
     * override the platform default encoding (if supported).
     */
    public static String toString(byte[] buf) {
        String encoding = System.getProperty("jna.encoding");
        String s = null;
        if (encoding != null) {
            try {
                s = new String(buf, encoding);
            }
            catch(UnsupportedEncodingException e) { }
        }
        if (s == null) {
            s = new String(buf);
        }
        int term = s.indexOf(0);
        if (term != -1)
            s = s.substring(0, term);
        return s;
    }

    /** Obtain a Java String from the given native wchar_t array.  If there is
     * no NUL terminator, the String will comprise the entire array.
     */
    public static String toString(char[] buf) {
        String s = new String(buf);
        int term = s.indexOf(0);
        if (term != -1)
            s = s.substring(0, term);
        return s;
    }

    /** Load a library interface from the given shared library, providing
     * the explicit interface class.
     */
    public static Object loadLibrary(String name, Class interfaceClass) {
        return loadLibrary(name, interfaceClass, Collections.EMPTY_MAP);
    }

    /** Load a library interface from the given shared library, providing
     * the explicit interface class and a map of options for the library.
     * If no library options are detected the map is interpreted as a map
     * of Java method names to native function names.
     * @param name
     * @param interfaceClass
     * @param libOptions Map of library options
     */
    public static Object loadLibrary(String name,
                                     Class interfaceClass,
                                     Map libOptions) {
        Library.Handler handler =
            new Library.Handler(name, interfaceClass, libOptions);
        ClassLoader loader = interfaceClass.getClassLoader();
        Library proxy = (Library)
            Proxy.newProxyInstance(loader, new Class[] {interfaceClass},
                handler);
        synchronized(libraries) {
            if (!libOptions.isEmpty())
                options.put(interfaceClass, libOptions);
            if (libOptions.containsKey(Library.OPTION_TYPE_MAPPER))
                typeMappers.put(interfaceClass, libOptions.get(Library.OPTION_TYPE_MAPPER));
            if (libOptions.containsKey(Library.OPTION_STRUCTURE_ALIGNMENT))
                alignments.put(interfaceClass, libOptions.get(Library.OPTION_STRUCTURE_ALIGNMENT));
            libraries.put(interfaceClass, new WeakReference(proxy));
        }
        return proxy;
    }

    /** Attempts to force initialization of an instance of the library interface
     * by loading a public static field of the requisite type.
     * Returns whether an instance variable was instantiated.
     * Expects that lock on libraries is already held
     */
    private static boolean loadInstance(Class cls) {
        if (libraries.containsKey(cls)) {
            return true;
        }
        if (cls != null) {
            try {
                Field[] fields = cls.getFields();
                for (int i=0;i < fields.length;i++) {
                    Field field = fields[i];
                    if (field.getType() == cls
                        && Modifier.isStatic(field.getModifiers())) {
                        // Ensure the field gets initialized
                        libraries.put(cls, new WeakReference(field.get(null)));
                        return true;
                    }
                }
            }
            catch (Exception e) {
                throw new IllegalArgumentException("Could not access instance of "
                    + cls + " (" + e + ")");
            }
        }
        return false;
    }

    /** Find the library interface corresponding to the given class.  Checks
     * all ancestor classes for a declaring class which implements
     * {@link Library}.
     */
    static Class findLibraryClass(Class cls) {
        if (cls == null) {
            return null;
        }
        if (Library.class.isAssignableFrom(cls)) {
            return cls;
        }
        Class fromDeclaring = findLibraryClass(cls.getDeclaringClass());
        if (fromDeclaring != null) {
            return fromDeclaring;
        }
        return findLibraryClass(cls.getSuperclass());
    }


    /** Return the preferred native library configuration options for the given
     * class.
     * @see Library
     */
    public static Map getLibraryOptions(Class type) {
        synchronized(libraries) {
            Class interfaceClass = findLibraryClass(type);
            if (interfaceClass == null)
                return null;
            if (!loadInstance(interfaceClass)
                || !options.containsKey(interfaceClass)) {
                try {
                    Field field = interfaceClass.getField("OPTIONS");
                    options.put(interfaceClass, field.get(null));
                }
                catch (NoSuchFieldException e) {
                }
                catch (Exception e) {
                    throw new IllegalArgumentException("OPTIONS must be a public field of type java.util.Map ("
                        + e + "): " + interfaceClass);
                }
            }
            return (Map)options.get(interfaceClass);
        }
    }

    /** Return the preferred {@link TypeMapper} for the given native interface.
     * @see Library#OPTION_TYPE_MAPPER
     */
    public static TypeMapper getTypeMapper(Class cls) {
        synchronized(libraries) {
            Class interfaceClass = findLibraryClass(cls);
            if (interfaceClass == null)
                return null;
            if (!loadInstance(interfaceClass)
                || !typeMappers.containsKey(interfaceClass)) {
                try {
                    Field field = interfaceClass.getField("TYPE_MAPPER");
                    typeMappers.put(interfaceClass, field.get(null));
                }
                catch (NoSuchFieldException e) {
                    Map options = getLibraryOptions(cls);
                    if (options != null
                        && options.containsKey(Library.OPTION_TYPE_MAPPER)) {
                        typeMappers.put(interfaceClass, options.get(Library.OPTION_TYPE_MAPPER));
                    }
                }
                catch (Exception e) {
                    throw new IllegalArgumentException("TYPE_MAPPER must be a public field of type "
                        + TypeMapper.class.getName() + " ("
                        + e + "): " + interfaceClass);
                }
            }
            return (TypeMapper)typeMappers.get(interfaceClass);
        }
    }

    /** Return the preferred structure alignment for the given native interface.
     * @see Library#OPTION_STRUCTURE_ALIGNMENT
     */
    public static int getStructureAlignment(Class cls) {
        synchronized(libraries) {
            Class interfaceClass = findLibraryClass(cls);
            if (interfaceClass == null)
                return Structure.ALIGN_DEFAULT;
            if (!loadInstance(interfaceClass)
                || !alignments.containsKey(interfaceClass)) {
                try {
                    Field field = interfaceClass.getField("STRUCTURE_ALIGNMENT");
                    alignments.put(interfaceClass, field.get(null));
                }
                catch(NoSuchFieldException e) {
                    Map options = getLibraryOptions(cls);
                    if (options != null
                        && options.containsKey(Library.OPTION_STRUCTURE_ALIGNMENT)) {
                        alignments.put(interfaceClass, options.get(Library.OPTION_STRUCTURE_ALIGNMENT));
                    }
                }
                catch(Exception e) {
                    throw new IllegalArgumentException("STRUCTURE_ALIGNMENT must be a public field of type int ("
                        + e + "): " + interfaceClass);
                }
            }
            Integer value = (Integer)alignments.get(interfaceClass);
            return value != null ? value.intValue() : Structure.ALIGN_DEFAULT;
        }
    }

    /** Return a byte array corresponding to the given String.  If the
     * system property <code>jna.encoding</code> is set, its value will override
     * the default platform encoding (if supported).
     */
    static byte[] getBytes(String s) {
        String encoding = System.getProperty("jna.encoding");
        if (encoding != null) {
            try {
                return s.getBytes(encoding);
            }
            catch (UnsupportedEncodingException e) {
            }
        }
        return s.getBytes();
    }

    private static String getNativeLibraryResourcePath() {
        String arch = System.getProperty("os.arch");
        String osPrefix;
        if (Platform.isWindows()) {
            osPrefix = "win32-" + arch;
        }
        else if (Platform.isMac()) {
            osPrefix = "darwin";
        }
        else if (Platform.isLinux()) {
            osPrefix = "linux-" + arch;
        }
        else if (Platform.isSolaris()) {
            osPrefix = "sunos-" + arch;
        }
        else {
            osPrefix = System.getProperty("os.name").toLowerCase();
            int space = osPrefix.indexOf(" ");
            if (space != -1) {
                osPrefix = osPrefix.substring(0, space);
            }
            osPrefix += "-" + arch;
        }
        return "/com/sun/jna/" + osPrefix;
    }

    private static void loadNativeLibrary() {
        String libname = System.mapLibraryName("jnidispatch");
        String resourceName = getNativeLibraryResourcePath() + "/" + libname;
        URL url = com.sun.jna.Native.class.getResource(resourceName);

        // Add an ugly hack for OpenJDK (soylatte) - JNI libs use the usual .dylib extension
        if (url == null && Platform.isMac() && resourceName.endsWith(".dylib")) {
            resourceName = resourceName.substring(0, resourceName.lastIndexOf(".dylib")) + ".jnilib";
            url = com.sun.jna.Native.class.getResource(resourceName);
        }
        if (url == null) {
            throw new UnsatisfiedLinkError("jnidispatch (" + resourceName
                + ") not found in resource path");
        }

        File lib = null;
        if (url.getProtocol().toLowerCase().equals("file")) {
            try {
                lib = new File(URLDecoder.decode(url.getPath(), "UTF8"));
            }
            catch(UnsupportedEncodingException e) {
                throw new Error("JRE is unexpectedly missing UTF8 encoding");
            }
        }
        else {
            InputStream is = com.sun.jna.Native.class.getResourceAsStream(resourceName);
            if (is == null) {
                throw new Error("Can't obtain jnidispatch InputStream");
            }

            FileOutputStream fos = null;
            try {
                // Suffix is required on windows, or library fails to load
                // Let Java pick the suffix
                lib = File.createTempFile("jna", null);
                lib.deleteOnExit();
                // Have to remove the temp file after VM exit on w32
                if (Platform.isWindows()) {
                    Runtime.getRuntime().addShutdownHook(new JnaConfiguration.W32Cleanup(lib));
                }
                fos = new FileOutputStream(lib);
                int count;
                byte[] buf = new byte[1024];
                while ((count = is.read(buf, 0, buf.length)) > 0) {
                    fos.write(buf, 0, count);
                }
            }
            catch(IOException e) {
                throw new Error("Failed to create temporary file for jnidispatch library", e);
            }
            finally {
                try { is.close(); } catch(IOException e) { }
                if (fos != null) {
                    try { fos.close(); } catch(IOException e) { }
                }
            }
        }
        System.load(lib.getAbsolutePath());
    }

    /**
     * Initialize field and method IDs for native methods of this class.
     * Returns the size of a native pointer.
     **/
    private static native int pointerSize();

    /** Return the size of a native <code>long</code>. */
    private static native int longSize();

    /** Return the size of a native <code>wchar_t</code>. */
    private static native int wideCharSize();

    private static native String getNativeVersion();
    private static native String getAPIChecksum();

    private static final ThreadLocal lastError = new ThreadLocal() {
        protected synchronized Object initialValue() {
            return new Integer(0);
        }
    };

    /** Retrieve the last error set by the OS.  This corresponds to
     * <code>GetLastError()</code> on Windows, and <code>errno</code> on
     * most other platforms.  The value is preserved per-thread, but whether
     * the original value is per-thread depends on the underlying OS.  The
     * result is undefined If {@link #getPreserveLastError} is
     * <code>false</code>.
     */
    public static int getLastError() {
        return ((Integer)lastError.get()).intValue();
    }

    /** Set the OS last error code.  Whether the setting is per-thread
     * or global depends on the underlying OS.
     */
    public static native void setLastError(int code);

    /** Update the last error value (called from native code). */
    static void updateLastError(int e) {
        lastError.set(new Integer(e));
    }

    /**
     * Returns a synchronized (thread-safe) library backed by the specified
     * library.  This wrapping will prevent simultaneous invocations of any
     * functions mapped to a given {@link NativeLibrary}.  Note that the
     * native library may still be sensitive to being called from different
     * threads.
     * <p>
     * @param  library the library to be "wrapped" in a synchronized library.
     * @return a synchronized view of the specified library.
     */
    public static Library synchronizedLibrary(final Library library) {
        Class cls = library.getClass();
        if (!Proxy.isProxyClass(cls)) {
            throw new IllegalArgumentException("Library must be a proxy class");
        }
        InvocationHandler ih = Proxy.getInvocationHandler(library);
        if (!(ih instanceof Library.Handler)) {
            throw new IllegalArgumentException("Unrecognized proxy handler: " + ih);
        }
        final Library.Handler handler = (Library.Handler)ih;
        InvocationHandler newHandler = new InvocationHandler() {
            public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
                synchronized(handler.getNativeLibrary()) {
                    return handler.invoke(library, method, args);
                }
            }
        };
        return (Library)Proxy.newProxyInstance(cls.getClassLoader(),
            cls.getInterfaces(),
            newHandler);
    }

    /** If running web start, determine the location of a given native
     * library.  This value may be used to properly set
     * <code>jna.library.path</code> so that JNA can load libraries identified
     * by the &lt;nativelib&gt; tag in the JNLP configuration file.  Returns
     * <code>null</code> if the Web Start native library cache location can not
     * be determined.
     * <p>
     * Use <code>System.getProperty("javawebstart.version")</code> to detect
     * whether your code is running under Web Start.
     * @throws IllegalArgumentException if the library can't be found by the
     * Web Start class loader, which usually means it wasn't included as
     * a <code>&lt;nativelib&gt;</code> resource in the JNLP file.
     */
    public static String getWebStartLibraryPath(String libName) {
        if (System.getProperty("javawebstart.version") == null)
            return null;
        try {
            ClassLoader cl = com.sun.jna.Native.class.getClassLoader();
            Method m = ClassLoader.class.getDeclaredMethod("findLibrary", new Class[] { String.class });
            m.setAccessible(true);
            String libpath = (String)m.invoke(cl, new Object[] { libName });
            if (libpath != null) {
                return new File(libpath).getParent();
            }
            String msg =
                "Library '" + libName + "' was not found by class loader " + cl;
            throw new IllegalArgumentException(msg);
        }
        catch (Exception e) {
            return null;
        }
    }

    /** For internal use only. */
    /* Windows won't allow file deletion while
     * it is in use, and the VM doesn't provide for explicit unloading of
     * native libraries (and the implicit method requires GC of a custom class
     * loader which loaded the class with native bits, which would require
     * all native bits to be encapsulated in a private class).
     * Instead, spawn a cleanup task to remove the file *after* the VM exits.
     */
    public static class W32Cleanup extends Thread {
        private File file;
        public W32Cleanup(File file) {
            this.file = file;
        }
        public void run() {
            try {
                Runtime.getRuntime().exec(new String[] {
                    System.getProperty("java.home") + "/bin/java",
                    "-cp", System.getProperty("java.class.path"),
                    getClass().getName(),
                    file.getAbsolutePath(),
                });
            }
            catch(IOException e) { e.printStackTrace(); }
        }
        public static void main(String[] args) {
            if (args.length == 1) {
                File file = new File(args[0]);
                if (file.exists()) {
                    long start = System.currentTimeMillis();
                    while (!file.delete() && file.exists()) {
                        try { Thread.sleep(10); }
                        catch(InterruptedException e) { }
                        if (System.currentTimeMillis() - start > 1000)
                            break;
                    }
                }
            }
            System.exit(0);
        }
    }

    /** Returns the native size for a given Java class.  Structures are
     * assumed to be <code>struct</code> pointers unless they implement
     * {@link Structure.ByValue}.
     */
    public static int getNativeSize(Class cls) {
        if (NativeMapped.class.isAssignableFrom(cls)) {
            cls = new NativeMappedConverter(cls).nativeType();
        }
        // boolean defaults to 32 bit integer if not otherwise mapped
        if (cls == boolean.class || cls == Boolean.class) return 4;
        if (cls == byte.class || cls == Byte.class) return 1;
        if (cls == short.class || cls == Short.class) return 2;
        if (cls == char.class || cls == Character.class) return WCHAR_SIZE;
        if (cls == int.class || cls == Integer.class) return 4;
        if (cls == long.class || cls == Long.class) return 8;
        if (cls == float.class || cls == Float.class) return 4;
        if (cls == double.class || cls == Double.class) return 8;
        if (Structure.class.isAssignableFrom(cls)) {
            if (Structure.ByValue.class.isAssignableFrom(cls)) {
                return Structure.newInstance(cls).size();
            }
            return POINTER_SIZE;
        }
        if (Pointer.class.isAssignableFrom(cls)
            || Buffer.class.isAssignableFrom(cls)
            || Callback.class.isAssignableFrom(cls)
            || String.class == cls
            || WString.class == cls) {
            return POINTER_SIZE;
        }
        throw new IllegalArgumentException("Native size for type \"" + cls.getName()
            + "\" is unknown");
    }

    /** Indicate whether the given class is supported as a native argument
     * type.
     */
    public static boolean isSupportedNativeType(Class cls) {
        if (Structure.class.isAssignableFrom(cls)) {
            return true;
        }
        try {
            return getNativeSize(cls) != 0;
        }
        catch(IllegalArgumentException e) {
            return false;
        }
    }

    /** Prints JNA library details to the console. */
    public static void main(String[] args) {
        Package pkg = com.sun.jna.Native.class.getPackage();
        System.out.println(pkg.getSpecificationTitle());
        System.out.println("Version: " + com.sun.jna.Native.class.getPackage().getImplementationVersion());
        System.out.println(" Native: " + getNativeVersion() + " ("
            + getAPIChecksum() + ")");
        System.exit(0);
    }
}
