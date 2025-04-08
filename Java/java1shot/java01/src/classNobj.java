public class classNobj {
    // to create an object
    // classname obj = new classname()
    String name;
    int level;

    classNobj(){
        level = 1;
    }

    classNobj(String Name, int Level){
        this.name = name;
        this.level = level;
    }

    void attack(){
        System.out.println(name + " attack!");
    }
}
