public fun <T> w_arrRemove(a: ArrayList<T>, i: Int, n: Int){ a.subList(i,i+n).clear(); }
public fun <T> w_arrSlice(a: ArrayList<T>, i: Int, n: Int): ArrayList<T> { return ArrayList<T>(a.subList(i,i+n)); }
public fun w_strSlice(a:String, i:Int, n:Int):String { return a.substring(i, i + n); }
public fun <K,V> w_mapGet(map: HashMap<K,V>, key: K, defaultValue: V): V? { val v: V? = map.get(key); return if ((v != null) || map.containsKey(key)) v else defaultValue; }
public fun w_BOOL(x: Int): Boolean { return x!=0; }
public fun w_INT(x: Boolean): Int { return if (x == true) 1 else 0; }
public fun w_NOT(x: Int): Int { return if (x==0) 1 else 0; }