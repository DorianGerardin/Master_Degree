#define GMAP_CORE
#include "gmap.hpp"

void question1a(){
    GMap mgmap;
    mgmap.add_dart();
    mgmap.add_dart();
    mgmap.add_dart();
    mgmap.print_alphas();
    assert(mgmap.darts().size() == 3);
    printf("add_dart seems valid\n");
}


void question1b(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 10; ++i) vlist.push_back(mgmap.add_dart());

    for(GMap::id_t vid : vlist) {
        for(int degree : {0,1,2}){
            assert(mgmap.alpha(degree,vid) == vid);
            assert(mgmap.is_free(degree,vid));
        }
    }
    printf("alpha and is_free seems valid\n");
}

void question1c(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 4; ++i) vlist.push_back(mgmap.add_dart());
    bool res = mgmap.link_darts(0, vlist[0], vlist[1]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[1], vlist[2]);
    assert(res == true);
    res = mgmap.link_darts(0, vlist[2], vlist[3]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[3], vlist[0]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[3], vlist[2]);
    assert(res == false and "darts are already linked");

    for(GMap::id_t vid : vlist) {
        assert(mgmap.alpha(0,vlist[0]) == vlist[1]);
        assert(mgmap.alpha(0,vlist[1]) == vlist[0]);
        assert(mgmap.alpha(0,vlist[2]) == vlist[3]);
        assert(mgmap.alpha(0,vlist[3]) == vlist[2]);

        assert(mgmap.alpha(1,vlist[0]) == vlist[3]);
        assert(mgmap.alpha(1,vlist[1]) == vlist[2]);
        assert(mgmap.alpha(1,vlist[2]) == vlist[1]);
        assert(mgmap.alpha(1,vlist[3]) == vlist[0]);

        assert(mgmap.alpha(2,vlist[0]) == vlist[0]);
        assert(mgmap.alpha(2,vlist[1]) == vlist[1]);
        assert(mgmap.alpha(2,vlist[2]) == vlist[2]);
        assert(mgmap.alpha(2,vlist[3]) == vlist[3]);

    }
    printf("link_darts  seems valid\n");
}


void question1d(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 4; ++i) vlist.push_back(mgmap.add_dart());
    assert(!mgmap.is_valid() && "check no point fix for alpha_0");
    mgmap.link_darts(0, vlist[0], vlist[1]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_0");  
    mgmap.link_darts(0, vlist[2], vlist[3]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_1");  
    mgmap.link_darts(1, vlist[1], vlist[2]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_1");  
    mgmap.link_darts(1, vlist[3], vlist[0]);
    assert(mgmap.is_valid());  
    mgmap.alphas[vlist[3]][1] = vlist[2];
    assert(!mgmap.is_valid() && "check if alpha_1 is involution");  
    mgmap.alphas[vlist[3]][1] = vlist[0];
    mgmap.alphas[vlist[0]][0] = vlist[2];
    assert(!mgmap.is_valid() && "check if alpha_0 is involution");  
    printf("is_valid seems valid\n");
}

template<class T>
void print_array(T array, const char * numtype = "%i"){
    printf("[");
    if (!array.empty()){
        if (array.size() > 1){
            for (typename T::const_iterator it = array.begin(); it != array.end()-1 ; ++it){
                printf(numtype,*it);
                printf(",");
            }
        }
        printf(numtype,*(array.end()-1));

    }
    printf("]");
}

void question2(){
    GMap mgmap = GMap::square();
    mgmap.print_alphas();
    std::vector<unsigned int> expected = {4,4,1};
    for (unsigned int i = 0 ; i < 3 ; ++i) {
        GMap::idlist_t ielements = mgmap.elements(i);
        printf( "Element de degree %i :",i);
        print_array(ielements);
        printf("\n");
        assert(ielements.size() == expected[i]);
    }
}

GMap3D question3(){
    GMap3D mgmap = GMap3D::square();
    mgmap.print_alphas();
    return mgmap;
}

GMap3D question4a(){
    GMap3D mgmap = GMap3D::cube();
    mgmap.print_alphas();
    return mgmap;
}

GMap3D question4b(){
    GMap3D mgmap = GMap3D::holeshape();
    mgmap.print_alphas();
    return mgmap;
}


void question5(){
    GMap3D mgmap = GMap3D::cube();
    printf("Euler characteristic : %i\n", mgmap.eulercharacteristic());
}


void question6a() {
    GMap3D mgmap = question4a();
    display(mgmap);
}

void question6b() {
    GMap3D mgmap = question4b();
    display(mgmap);
}



void question7(){
    GMap3D mgmap = GMap3D::cube();
    mgmap.print_alphas();
    mgmap = mgmap.dual();
    mgmap.print_alphas();
    //display(mgmap);
}

#define CHECK(I, target) \
            if (strcmp(selection,I) == 0){ \
                printf("Selected question :%s\n",selection); \
                target(); \
            } \

int main(int argc, char ** argv){
    const char * selection = "1";
    if(argc == 2) {
        selection = argv[1];
    }

    CHECK("1a", question1a)
    else CHECK("1b", question1b)
    else CHECK("1c", question1c)
    else CHECK("1d", question1d)
    else CHECK("2", question2)
    else CHECK("3", question3)
    else CHECK("4a", question4a)
    else CHECK("4b", question4b)
    else CHECK("5", question5)
    else CHECK("6a", question6a)
    else CHECK("6b", question6b)
    else CHECK("7", question7)
    else {
            printf("Default question 1a. Unknown option :%s\n",selection);
            question1a();
        }
    return 0;
}
