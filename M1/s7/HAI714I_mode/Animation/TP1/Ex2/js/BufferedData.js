class BufferedData {
//pourquoi le type des données a-t-il besoin d'un contexte pour être défini?
//on définit un local type qui n'en a pas besoin, et quand le shader a besoin du type
//on utilise son contexte pour lui donner le type. 
    constructor(data, localType, dimension){
        this.data = data;
        this.type = localType;
        this.dimension = dimension;
    }

    size(){
        if (!this.data) return 0;
        return this.data.length / this.dimension;
    }

//les types sont rappelés dans la doc: https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Types
//on ne met que ceux qui sont utiles dans cette classe...à mettre à jour au besoin...
    getType(context){
        switch (this.type){
            case bdt.float: //32 bits ieee floating point
                return context.FLOAT;
            case bdt.int: //signed 32 bits
                return context.INT;
            case bdt.ushort: // unsigned 16 bits
                return context.UNSIGNED_SHORT;
            case bdt.ubyte: // unsigned 8 bits
                return context.UNSIGNED_BYTE;
            default:
                //gerer les exceptions?
                return undefined;
        }
    }
    
    //tailles des types exprimées en octets (bytes) d'après 
    //la doc https://developer.mozilla.org/fr/docs/Web/JavaScript/Typed_arrays
    getSizeOfType(context){
        switch (this.type){
            case bdt.float: //32 bits ieee floating point
                return 4;
            case bdt.int: //signed 32 bits (long)
                return 4;
            case bdt.ushort: // unsigned 16 bits
                return 2;
            case bdt.ubyte: // unsigned 8 bits
                return 1;
            default:
                //gerer les exceptions?
                return undefined;
        }
        
    }
}