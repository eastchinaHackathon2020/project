package com.mycompany.myapp.util;

import java.util.Comparator;
import java.util.HashMap;

//按照名字的升序排列    实现接口       泛型是自定义类，里面有要排序的内容
public class myHashSort implements Comparator{
        //按照姓名的升序排列，前面加个负号就按照降序排列

    @Override
    public int compare(Object o1, Object o2) {
        o1 = (HashMap<String,Object>) o1;
        o2 = (HashMap<String,Object>) o2;
        return ((String)(((HashMap<?, ?>) o1).get("label"))).compareTo((String)(((HashMap<?, ?>) o2).get("label")));
    }
}
