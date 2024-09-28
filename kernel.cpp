__asm("jmp kmain"); // transfer to the main

#define LETTERS_COUNT 26
#define NULL 0

#define GDT_CS (0x8)
#define IDT_TYPE_INTR (0x0E)
#define IDT_TYPE_TRAP (0x0F)
#define PIC1_PORT (0x20)

#define VIDEO_BUF_PTR (0xb8000) // video buffer
#define CURSOR_PORT (0x3D4) // the basic cursor control port of the text screen
#define VIDEO_WIDTH (80) // sizes of the text screen
#define VIDEO_LEN (25)
#define WHITE (0x07) // color of text

#define OFFSET (0x80)
#define SPACE (0x29)
#define SHIFT (0x2A)
#define BACKSPACE (0x0E)
#define ENTER (0x1C)

/* Some useful data */

struct idt_entry
{
	unsigned short base_lo; 
	unsigned short segm_sel; 
	unsigned char always0; 
	unsigned char flags; 
	unsigned short base_hi; 
} __attribute__((packed)); 
struct idt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)); 
typedef void (*intr_handler)();
struct idt_entry g_idt[256]; 
struct idt_ptr g_idtp; 

typedef struct {
    char letter; 
    char* words_pairs[20][2];
    unsigned int word_count; 
    bool allow_flag;
}DictSection;
typedef struct {
    DictSection* sections[LETTERS_COUNT]; 
    unsigned int words_count; 
    unsigned int active_count;
    unsigned char letters_array[LETTERS_COUNT]; 
} Dictionary;

static DictSection section_a = {'a', \
                     {{(char*)"about", (char*)"acerca"}, \
                     {(char*)"above", (char*)"encima"}, \
                     {(char*)"accept", (char*)"aceptar"}, \
                     {(char*)"account", (char*)"cuenta"}, \
                     {(char*)"achieve", (char*)"lograr"}, \
                     {(char*)"action", (char*)"acción"}, \
                     {(char*)"active", (char*)"activo"}, \
                     {(char*)"actor", (char*)"actor"}, \
                     {(char*)"add", (char*)"agregar"}, \
                     {(char*)"address", (char*)"dirección"}, \
                     {(char*)"advance", (char*)"avanzar"}, \
                     {(char*)"advice", (char*)"consejo"}, \
                     {(char*)"afford", (char*)"permitir"}, \
                     {(char*)"all", (char*)"todo"}, \
                     {(char*)"allow", (char*)"permitir"}, \
                     {(char*)"almost", (char*)"casi"}, \
                     {(char*)"alone", (char*)"solo"}, \
                     {(char*)"already", (char*)"ya"}}, \
                    18, false};

static DictSection section_b = {'b', \
                     {{(char*)"baby", (char*)"bebé"}, \
                     {(char*)"back", (char*)"espalda"}, \
                     {(char*)"bad", (char*)"malo"}, \
                     {(char*)"bag", (char*)"bolsa"}, \
                     {(char*)"ball", (char*)"pelota"}, \
                     {(char*)"bank", (char*)"banco"}, \
                     {(char*)"bar", (char*)"barra"}, \
                     {(char*)"base", (char*)"base"}, \
                     {(char*)"be", (char*)"ser/estar"}, \
                     {(char*)"bear", (char*)"soportar"}, \
                     {(char*)"beautiful", (char*)"hermoso"}, \
                     {(char*)"become", (char*)"convertirse"}, \
                     {(char*)"begin", (char*)"comenzar"}, \
                     {(char*)"believe", (char*)"creer"}, \
                     {(char*)"best", (char*)"mejor"}, \
                     {(char*)"bird", (char*)"pájaro"}, \
                     {(char*)"black", (char*)"negro"}, \
                     {(char*)"block", (char*)"bloquear"}, \
                     {(char*)"blue", (char*)"azul"}}, \
                    19, false};

static DictSection section_c = {'c', \
                    {{(char*)"call", (char*)"llamar"}, \
                    {(char*)"can", (char*)"poder"}, \
                    {(char*)"car", (char*)"coche"}, \
                    {(char*)"care", (char*)"cuidado"}, \
                    {(char*)"carry", (char*)"llevar"}, \
                    {(char*)"case", (char*)"caso"}, \
                    {(char*)"cat", (char*)"gato"}, \
                    {(char*)"catch", (char*)"atrapar"}, \
                    {(char*)"cause", (char*)"causa"}, \
                    {(char*)"change", (char*)"cambiar"}, \
                    {(char*)"check", (char*)"comprobar"}, \
                    {(char*)"child", (char*)"niño"}, \
                    {(char*)"city", (char*)"ciudad"}, \
                    {(char*)"class", (char*)"clase"}, \
                    {(char*)"clean", (char*)"limpiar"}, \
                    {(char*)"common", (char*)"común"}, \
                    {(char*)"company", (char*)"compañía"}, \
                    {(char*)"complete", (char*)"completo"}, \
                    {(char*)"continue", (char*)"continuar"}}, \
                    19, false};

static DictSection section_d = {'d', \
                    {{(char*)"dance", (char*)"bailar"}, \
                     {(char*)"dark", (char*)"oscuro"}, \
                     {(char*)"day", (char*)"día"}, \
                     {(char*)"dear", (char*)"querido"}, \
                     {(char*)"decide", (char*)"decidir"}, \
                     {(char*)"deep", (char*)"profundo"}, \
                     {(char*)"defend", (char*)"defender"}, \
                     {(char*)"define", (char*)"definir"}, \
                     {(char*)"delightful", (char*)"delicioso"}, \
                     {(char*)"depend", (char*)"depender"}, \
                     {(char*)"describe", (char*)"describir"}, \
                     {(char*)"difficult", (char*)"difícil"}, \
                     {(char*)"dinner", (char*)"cena"}, \
                     {(char*)"directly", (char*)"directamente"}, \
                     {(char*)"discover", (char*)"descubrir"}, \
                     {(char*)"discuss", (char*)"discutir"}, \
                     {(char*)"distance", (char*)"distancia"}, \
                     {(char*)"doctor", (char*)"médico"}, \
                     {(char*)"door", (char*)"puerta"}}, \
                     19, false};

static DictSection section_e = {'e', \
                    {{(char*)"each", (char*)"cada"}, \
                     {(char*)"early", (char*)"temprano"}, \
                     {(char*)"easy", (char*)"fácil"}, \
                     {(char*)"eat", (char*)"comer"}, \
                     {(char*)"economic", (char*)"económico"}, \
                     {(char*)"education", (char*)"educación"}, \
                     {(char*)"effect", (char*)"efecto"}, \
                     {(char*)"effort", (char*)"esfuerzo"}, \
                     {(char*)"eight", (char*)"ocho"}, \
                     {(char*)"either", (char*)"cualquiera"}, \
                     {(char*)"electric", (char*)"eléctrico"}, \
                     {(char*)"element", (char*)"elemento"}, \
                     {(char*)"else", (char*)"otro"}, \
                     {(char*)"end", (char*)"fin"}, \
                     {(char*)"even", (char*)"incluso"}, \
                     {(char*)"ever", (char*)"siempre"}, \
                     {(char*)"every", (char*)"cada"}, \
                     {(char*)"everything", (char*)"todo"}, \
                     {(char*)"example", (char*)"ejemplo"}}, \
                    19, false};

static DictSection section_f = {'f', \
                    {{(char*)"face", (char*)"cara"}, \
                     {(char*)"fact", (char*)"hecho"}, \
                     {(char*)"fail", (char*)"fallar"}, \
                     {(char*)"fall", (char*)"caer"}, \
                     {(char*)"family", (char*)"familia"}, \
                     {(char*)"far", (char*)"lejos"}, \
                     {(char*)"fast", (char*)"rápido"}, \
                     {(char*)"father", (char*)"padre"}, \
                     {(char*)"fear", (char*)"miedo"}, \
                     {(char*)"feel", (char*)"sentir"}, \
                     {(char*)"few", (char*)"pocos"}, \
                     {(char*)"find", (char*)"encontrar"}, \
                     {(char*)"first", (char*)"primero"}, \
                     {(char*)"five", (char*)"cinco"}, \
                     {(char*)"floor", (char*)"suelo"}, \
                     {(char*)"fly", (char*)"volar"}, \
                     {(char*)"follow", (char*)"seguir"}, \
                     {(char*)"food", (char*)"comida"}, \
                     {(char*)"for", (char*)"para"}}, \
                    19, false};

static DictSection section_g = {'g', \
                    {{(char*)"game", (char*)"juego"}, \
                     {(char*)"garden", (char*)"jardín"}, \
                     {(char*)"gas", (char*)"gas"}, \
                     {(char*)"general", (char*)"general"}, \
                     {(char*)"get", (char*)"obtener"}, \
                     {(char*)"give", (char*)"dar"}, \
                     {(char*)"go", (char*)"ir"}, \
                     {(char*)"good", (char*)"bueno"}, \
                     {(char*)"great", (char*)"genial"}, \
                     {(char*)"group", (char*)"grupo"}, \
                     {(char*)"grow", (char*)"crecer"}, \
                     {(char*)"guess", (char*)"adivinar"}, \
                     {(char*)"gun", (char*)"arma"}, \
                     {(char*)"guy", (char*)"chico"}, \
                     {(char*)"goal", (char*)"meta"}, \
                     {(char*)"gold", (char*)"oro"}, \
                     {(char*)"goodbye", (char*)"adiós"}, \
                     {(char*)"greatly", (char*)"en gran medida"}, \
                     {(char*)"green", (char*)"verde"}}, \
                    19, false};

static DictSection section_h = {'h', \
                    {{(char*)"had", (char*)"tenía"}, \
                     {(char*)"her", (char*)"su (de ella)"}, \
                     {(char*)"here", (char*)"aquí"}, \
                     {(char*)"high", (char*)"alto"}, \
                     {(char*)"his", (char*)"su (de él)"}, \
                     {(char*)"history", (char*)"historia"}, \
                     {(char*)"home", (char*)"hogar"}, \
                     {(char*)"hope", (char*)"esperanza"}, \
                     {(char*)"hour", (char*)"hora"}, \
                     {(char*)"house", (char*)"casa"}, \
                     {(char*)"how", (char*)"cómo"}, \
                     {(char*)"human", (char*)"humano"}, \
                     {(char*)"hundred", (char*)"cien"}, \
                     {(char*)"hurt", (char*)"herir"}, \
                     {(char*)"hurry", (char*)"apresurarse"}, \
                     {(char*)"husband", (char*)"esposo"}}, \
                    19, false};

static DictSection section_i = {'i', \
                    {{(char*)"idea", (char*)"idea"}, \
                     {(char*)"important", (char*)"importante"}, \
                     {(char*)"improve", (char*)"mejorar"}, \
                     {(char*)"include", (char*)"incluir"}, \
                     {(char*)"interest", (char*)"interés"}, \
                     {(char*)"inspire", (char*)"inspirar"}, \
                     {(char*)"increase", (char*)"aumentar"}, \
                     {(char*)"invite", (char*)"invitar"}, \
                     {(char*)"impact", (char*)"impacto"}, \
                     {(char*)"illustrate", (char*)"ilustrar"}, \
                     {(char*)"intention", (char*)"intención"}, \
                     {(char*)"influence", (char*)"influencia"}, \
                     {(char*)"integrate", (char*)"integrar"}, \
                     {(char*)"individual", (char*)"individual"}, \
                     {(char*)"initiate", (char*)"iniciar"}, \
                     {(char*)"innovation", (char*)"innovación"}, \
                     {(char*)"integrity", (char*)"integridad"}, \
                     {(char*)"interaction", (char*)"interacción"}, \
                     {(char*)"initiative", (char*)"iniciativa"}}, \
                    19, false};

static DictSection section_j = {'j', \
                    {{(char*)"job", (char*)"trabajo"}, \
                     {(char*)"join", (char*)"unir"}, \
                     {(char*)"journey", (char*)"viaje"}, \
                     {(char*)"judge", (char*)"juzgar"}, \
                     {(char*)"just", (char*)"justo"}, \
                     {(char*)"joke", (char*)"broma"}, \
                     {(char*)"jump", (char*)"saltar"}, \
                     {(char*)"jewel", (char*)"joya"}, \
                     {(char*)"journal", (char*)"diario"}, \
                     {(char*)"jungle", (char*)"jungla"}, \
                     {(char*)"justice", (char*)"justicia"}, \
                     {(char*)"jovial", (char*)"jovial"}, \
                     {(char*)"jeopardy", (char*)"peligro"}, \
                     {(char*)"jargon", (char*)"jerga"}, \
                     {(char*)"junction", (char*)"cruce"}, \
                     {(char*)"jacket", (char*)"chaqueta"}, \
                     {(char*)"jam", (char*)"mermelada"}, \
                     {(char*)"jeans", (char*)"jeans"}, \
                     {(char*)"jigsaw", (char*)"rompecabezas"}}, \
                    19, false};

static DictSection section_k = {'k', \
                    {{(char*)"kind", (char*)"amable"}, \
                     {(char*)"knot", (char*)"nudo"}, \
                     {(char*)"kid", (char*)"niño"}, \
                     {(char*)"kit", (char*)"kit"}, \
                     {(char*)"keen", (char*)"entusiasta"}, \
                     {(char*)"knight", (char*)"caballero"}, \
                     {(char*)"knee", (char*)"rodilla"}, \
                     {(char*)"knockout", (char*)"nocaut"}, \
                     {(char*)"kindness", (char*)"bondad"}, \
                     {(char*)"kernel", (char*)"núcleo"}, \
                     {(char*)"kingdom", (char*)"reino"}, \
                     {(char*)"kettle", (char*)"tetera"}, \
                     {(char*)"kilometer", (char*)"kilómetro"}, \
                     {(char*)"kiss", (char*)"beso"}, \
                     {(char*)"knowledgeable", (char*)"sabio"}}, \
                    19, false};

static DictSection section_l = {'l', \
                    {{(char*)"love", (char*)"amor"}, \
                     {(char*)"learn", (char*)"aprender"}, \
                     {(char*)"light", (char*)"luz"}, \
                     {(char*)"life", (char*)"vida"}, \
                     {(char*)"listen", (char*)"escuchar"}, \
                     {(char*)"lead", (char*)"liderar"}, \
                     {(char*)"link", (char*)"enlace"}, \
                     {(char*)"laugh", (char*)"reír"}, \
                     {(char*)"local", (char*)"local"}, \
                     {(char*)"language", (char*)"idioma"}, \
                     {(char*)"limit", (char*)"límite"}, \
                     {(char*)"logic", (char*)"lógica"}, \
                     {(char*)"long", (char*)"largo"}, \
                     {(char*)"lose", (char*)"perder"}, \
                     {(char*)"lift", (char*)"levantar"}, \
                     {(char*)"look", (char*)"mirar"}, \
                     {(char*)"level", (char*)"nivel"}, \
                     {(char*)"lighten", (char*)"aligerar"}, \
                     {(char*)"lifestyle", (char*)"estilo de vida"}}, \
                    19, false};

static DictSection section_m = {'m', \
                    {{(char*)"make", (char*)"hacer"}, \
                     {(char*)"manage", (char*)"gestionar"}, \
                     {(char*)"motivate", (char*)"motivar"}, \
                     {(char*)"moment", (char*)"momento"}, \
                     {(char*)"mind", (char*)"mente"}, \
                     {(char*)"move", (char*)"mover"}, \
                     {(char*)"method", (char*)"método"}, \
                     {(char*)"meet", (char*)"encontrarse"}, \
                     {(char*)"market", (char*)"mercado"}, \
                     {(char*)"measure", (char*)"medir"}, \
                     {(char*)"maintain", (char*)"mantener"}, \
                     {(char*)"material", (char*)"material"}, \
                     {(char*)"mission", (char*)"misión"}, \
                     {(char*)"mystery", (char*)"misterio"}, \
                     {(char*)"mirror", (char*)"espejo"}, \
                     {(char*)"merge", (char*)"fusionar"}, \
                     {(char*)"modern", (char*)"moderno"}, \
                     {(char*)"message", (char*)"mensaje"}, \
                     {(char*)"magnify", (char*)"magnificar"}}, \
                    19, false};

static DictSection section_n = {'n', \
                    {{(char*)"name", (char*)"nombre"}, \
                     {(char*)"new", (char*)"nuevo"}, \
                     {(char*)"need", (char*)"necesitar"}, \
                     {(char*)"notice", (char*)"notar"}, \
                     {(char*)"next", (char*)"siguiente"}, \
                     {(char*)"nature", (char*)"naturaleza"}, \
                     {(char*)"nurture", (char*)"nutrir"}, \
                     {(char*)"network", (char*)"red"}, \
                     {(char*)"navigate", (char*)"navegar"}, \
                     {(char*)"negotiate", (char*)"negociar"}, \
                     {(char*)"neutral", (char*)"neutral"}, \
                     {(char*)"noble", (char*)"noble"}, \
                     {(char*)"novel", (char*)"novela"}, \
                     {(char*)"normal", (char*)"normal"}, \
                     {(char*)"narrate", (char*)"narrar"}, \
                     {(char*)"niche", (char*)"nicho"}, \
                     {(char*)"native", (char*)"nativo"}, \
                     {(char*)"noticeable", (char*)"notable"}, \
                     {(char*)"neighborhood", (char*)"vecindario"}}, \
                    19, false};

static DictSection section_o = {'o', \
                    {{(char*)"open", (char*)"abrir"}, \
                     {(char*)"offer", (char*)"ofrecer"}, \
                     {(char*)"observe", (char*)"observar"}, \
                     {(char*)"organize", (char*)"organizar"}, \
                     {(char*)"optimize", (char*)"optimizar"}, \
                     {(char*)"ocean", (char*)"océano"}, \
                     {(char*)"outcome", (char*)"resultado"}, \
                     {(char*)"opinion", (char*)"opinión"}, \
                     {(char*)"original", (char*)"original"}, \
                     {(char*)"obstacle", (char*)"obstáculo"}, \
                     {(char*)"operation", (char*)"operación"}, \
                     {(char*)"overcome", (char*)"superar"}, \
                     {(char*)"object", (char*)"objeto"}, \
                     {(char*)"outreach", (char*)"alcance"}, \
                     {(char*)"offerings", (char*)"ofertas"}, \
                     {(char*)"ownership", (char*)"propiedad"}, \
                     {(char*)"output", (char*)"salida"}, \
                     {(char*)"oversee", (char*)"supervisar"}, \
                     {(char*)"obscure", (char*)"oscuro"}}, \
                    19, false};

static DictSection section_p = {'p', \
                    {{(char*)"place", (char*)"lugar"}, \
                     {(char*)"provide", (char*)"proveer"}, \
                     {(char*)"perform", (char*)"realizar"}, \
                     {(char*)"prepare", (char*)"preparar"}, \
                     {(char*)"progress", (char*)"progreso"}, \
                     {(char*)"project", (char*)"proyecto"}, \
                     {(char*)"process", (char*)"proceso"}, \
                     {(char*)"participate", (char*)"participar"}, \
                     {(char*)"power", (char*)"poder"}, \
                     {(char*)"potential", (char*)"potencial"}, \
                     {(char*)"promote", (char*)"promover"}, \
                     {(char*)"policy", (char*)"política"}, \
                     {(char*)"produce", (char*)"producir"}, \
                     {(char*)"pattern", (char*)"patrón"}, \
                     {(char*)"presence", (char*)"presencia"}, \
                     {(char*)"preserve", (char*)"preservar"}, \
                     {(char*)"priority", (char*)"prioridad"}, \
                     {(char*)"passion", (char*)"pasión"}, \
                     {(char*)"precision", (char*)"precisión"}}, \
                    19, false};

static DictSection section_q = {'q', \
                    {{(char*)"question", (char*)"pregunta"}, \
                     {(char*)"quickly", (char*)"rápidamente"}, \
                     {(char*)"quote", (char*)"cita"}, \
                     {(char*)"quiet", (char*)"tranquilo"}, \
                     {(char*)"quota", (char*)"cuota"}, \
                     {(char*)"quaint", (char*)"peculiar"}, \
                     {(char*)"quest", (char*)"búsqueda"}, \
                     {(char*)"quench", (char*)"apagar"}, \
                     {(char*)"qualify", (char*)"calificar"}, \
                     {(char*)"quarantine", (char*)"cuarentena"}, \
                     {(char*)"quarrel", (char*)"disputa"}, \
                     {(char*)"quintessential", (char*)"esencial"}, \
                     {(char*)"quizzical", (char*)"inquisitivo"}, \
                     {(char*)"quibble", (char*)"discutir"}, \
                     {(char*)"quirk", (char*)"peculiaridad"}, \
                     {(char*)"quicksand", (char*)"arena movediza"}, \
                     {(char*)"quasar", (char*)"cuasar"}}, \
                    19, false};

static DictSection section_r = {'r', \
                    {{(char*)"research", (char*)"investigación"}, \
                     {(char*)"result", (char*)"resultado"}, \
                     {(char*)"reveal", (char*)"revelar"}, \
                     {(char*)"respond", (char*)"responder"}, \
                     {(char*)"require", (char*)"requerir"}, \
                     {(char*)"reform", (char*)"reforma"}, \
                     {(char*)"regulate", (char*)"regular"}, \
                     {(char*)"reflect", (char*)"reflejar"}, \
                     {(char*)"relationship", (char*)"relación"}, \
                     {(char*)"resource", (char*)"recurso"}, \
                     {(char*)"review", (char*)"revisión"}, \
                     {(char*)"recognize", (char*)"reconocer"}, \
                     {(char*)"reduce", (char*)"reducir"}, \
                     {(char*)"retain", (char*)"mantener"}, \
                     {(char*)"reward", (char*)"recompensa"}, \
                     {(char*)"risk", (char*)"riesgo"}, \
                     {(char*)"reliable", (char*)"fiable"}, \
                     {(char*)"reach", (char*)"alcanzar"}, \
                     {(char*)"reputation", (char*)"reputación"}}, \
                    19, false};

static DictSection section_s = {'s', \
                    {{(char*)"system", (char*)"sistema"}, \
                     {(char*)"strategy", (char*)"estrategia"}, \
                     {(char*)"support", (char*)"apoyo"}, \
                     {(char*)"solution", (char*)"solución"}, \
                     {(char*)"significant", (char*)"significativo"}, \
                     {(char*)"success", (char*)"éxito"}, \
                     {(char*)"sustain", (char*)"sostener"}, \
                     {(char*)"structure", (char*)"estructura"}, \
                     {(char*)"service", (char*)"servicio"}, \
                     {(char*)"scope", (char*)"alcance"}, \
                     {(char*)"standard", (char*)"estándar"}, \
                     {(char*)"safety", (char*)"seguridad"}, \
                     {(char*)"situation", (char*)"situación"}, \
                     {(char*)"specify", (char*)"especificar"}, \
                     {(char*)"strength", (char*)"fortaleza"}, \
                     {(char*)"source", (char*)"fuente"}, \
                     {(char*)"schedule", (char*)"horario"}, \
                     {(char*)"search", (char*)"buscar"}, \
                     {(char*)"stimulate", (char*)"estimular"}}, \
                    19, false};

static DictSection section_t = {'t', \
                    {{(char*)"team", (char*)"equipo"}, \
                     {(char*)"technology", (char*)"tecnología"}, \
                     {(char*)"test", (char*)"prueba"}, \
                     {(char*)"time", (char*)"tiempo"}, \
                     {(char*)"tool", (char*)"herramienta"}, \
                     {(char*)"treatment", (char*)"tratamiento"}, \
                     {(char*)"trend", (char*)"tendencia"}, \
                     {(char*)"target", (char*)"objetivo"}, \
                     {(char*)"tactic", (char*)"táctica"}, \
                     {(char*)"transfer", (char*)"transferir"}, \
                     {(char*)"trust", (char*)"confianza"}, \
                     {(char*)"type", (char*)"tipo"}, \
                     {(char*)"tolerance", (char*)"tolerancia"}, \
                     {(char*)"track", (char*)"rastrear"}, \
                     {(char*)"transform", (char*)"transformar"}, \
                     {(char*)"theory", (char*)"teoría"}, \
                     {(char*)"task", (char*)"tarea"}, \
                     {(char*)"topic", (char*)"tema"}, \
                     {(char*)"template", (char*)"plantilla"}}, \
                    19, false};

static DictSection section_u = {'u', \
                    {{(char*)"understanding", (char*)"comprensión"}, \
                     {(char*)"utility", (char*)"utilidad"}, \
                     {(char*)"user", (char*)"usuario"}, \
                     {(char*)"usage", (char*)"uso"}, \
                     {(char*)"update", (char*)"actualización"}, \
                     {(char*)"upgrade", (char*)"mejora"}, \
                     {(char*)"universe", (char*)"universo"}, \
                     {(char*)"unit", (char*)"unidad"}, \
                     {(char*)"universal", (char*)"universal"}, \
                     {(char*)"urgency", (char*)"urgencia"}, \
                     {(char*)"unique", (char*)"único"}, \
                     {(char*)"upcoming", (char*)"próximo"}, \
                     {(char*)"unify", (char*)"unificar"}, \
                     {(char*)"understand", (char*)"entender"}, \
                     {(char*)"utilize", (char*)"utilizar"}, \
                     {(char*)"unveil", (char*)"revelar"}, \
                     {(char*)"unfold", (char*)"desplegar"}, \
                     {(char*)"unravel", (char*)"desentrañar"}}, \
                    19, false};

static DictSection section_v = {'v', \
                    {{(char*)"value", (char*)"valor"}, \
                     {(char*)"venture", (char*)"empresa"}, \
                     {(char*)"volume", (char*)"volumen"}, \
                     {(char*)"vote", (char*)"voto"}, \
                     {(char*)"vulnerability", (char*)"vulnerabilidad"}, \
                     {(char*)"verify", (char*)"verificar"}, \
                     {(char*)"viable", (char*)"viable"}, \
                     {(char*)"vocabulary", (char*)"vocabulario"}, \
                     {(char*)"victory", (char*)"victoria"}, \
                     {(char*)"visual", (char*)"visual"}, \
                     {(char*)"volunteer", (char*)"voluntario"}, \
                     {(char*)"vortex", (char*)"vórtice"}, \
                     {(char*)"vigilance", (char*)"vigilancia"}, \
                     {(char*)"virtual", (char*)"virtual"}, \
                     {(char*)"versatile", (char*)"versátil"}, \
                     {(char*)"vanguard", (char*)"vanguardia"}}, \
                    19, false};

static DictSection section_w = {'w', \
                    {{(char*)"work", (char*)"trabajo"}, \
                     {(char*)"world", (char*)"mundo"}, \
                     {(char*)"wealth", (char*)"riqueza"}, \
                     {(char*)"way", (char*)"manera"}, \
                     {(char*)"wonder", (char*)"maravilla"}, \
                     {(char*)"witness", (char*)"testigo"}, \
                     {(char*)"wisdom", (char*)"sabiduría"}, \
                     {(char*)"welfare", (char*)"bienestar"}, \
                     {(char*)"wave", (char*)"ola"}, \
                     {(char*)"write", (char*)"escribir"}, \
                     {(char*)"web", (char*)"red"}, \
                     {(char*)"win", (char*)"ganar"}, \
                     {(char*)"warning", (char*)"advertencia"}, \
                     {(char*)"wholesale", (char*)"mayorista"}, \
                     {(char*)"wish", (char*)"deseo"}, \
                     {(char*)"wonderful", (char*)"maravilloso"}, \
                     {(char*)"worth", (char*)"valor"}, \
                     {(char*)"workshop", (char*)"taller"}, \
                     {(char*)"widespread", (char*)"generalizado"}}, \
                    19, false};

static DictSection section_x = {'x', \
                    {{(char*)"x-ray", (char*)"rayos X"}, \
                    {(char*)"xenon", (char*)"xenón"}, \
                    {(char*)"xerox", (char*)"xerografía"}, \
                    {(char*)"xylophone", (char*)" xilófono"}, \
                    {(char*)"x-axis", (char*)"eje X"}, \
                    {(char*)"xenophobia", (char*)"xenofobia"}, \
                    {(char*)"xenolith", (char*)"xenolito"}, \
                    {(char*)"xiphoid", (char*)"xifoides"}, \
                    {(char*)"xylograph", (char*)"xilografía"}, \
                    {(char*)"xylose", (char*)"xilosa"}, \
                    {(char*)"xanthic", (char*)"xántico"}, \
                    {(char*)"xtreme", (char*)"extremo"}, \
                    {(char*)"xylophonist", (char*)"xilofonista"}, \
                    {(char*)"xystus", (char*)"xistus"}, \
                    {(char*)"xenobiotic", (char*)"xenobiótico"}, \
                    {(char*)"xylenes", (char*)"xilenos"}, \
                    {(char*)"x-factor", (char*)"factor X"}, \
                    {(char*)"xeriscape", (char*)"xeriscape"}, \
                    {(char*)"xylophonically", (char*)"xilofónicamente"}}, \
                    19, false};

static DictSection section_y = {'y', \
                    {{(char*)"yes", (char*)"sí"}, \
                    {(char*)"yellow", (char*)"amarillo"}, \
                    {(char*)"yesterday", (char*)"ayer"}, \
                    {(char*)"young", (char*)"joven"}, \
                    {(char*)"your", (char*)"tu"}, \
                    {(char*)"yours", (char*)"tuyo"}, \
                    {(char*)"yoga", (char*)"yoga"}, \
                    {(char*)"yard", (char*)"jardín"}, \
                    {(char*)"year", (char*)"año"}, \
                    {(char*)"yacht", (char*)"yate"}, \
                    {(char*)"yummy", (char*)"delicioso"}, \
                    {(char*)"yoke", (char*)"yugo"}, \
                    {(char*)"yield", (char*)"rendimiento"}, \
                    {(char*)"yarn", (char*)"hilo"}, \
                    {(char*)"yet", (char*)"todavía"}, \
                    {(char*)"youth", (char*)"juventud"}, \
                    {(char*)"yonder", (char*)"allá"}, \
                    {(char*)"yo-yo", (char*)"yo-yo"}}, \
                    19, false};

static DictSection section_z = {'z', \
                    {{(char*)"zebra", (char*)"cebra"}, \
                    {(char*)"zoom", (char*)"acercar"}, \
                    {(char*)"zip", (char*)"cerrar"}, \
                    {(char*)"zone", (char*)"zona"}, \
                    {(char*)"zeal", (char*)"entusiasmo"}, \
                    {(char*)"zenith", (char*)"cenit"}, \
                    {(char*)"zest", (char*)"entusiasmo"}, \
                    {(char*)"zigzag", (char*)"zigzag"}, \
                    {(char*)"zephyr", (char*)"céfiro"}, \
                    {(char*)"zen", (char*)"zen"}, \
                    {(char*)"zodiac", (char*)"zodíaco"}, \
                    {(char*)"zany", (char*)"loco"}, \
                    {(char*)"zap", (char*)"golpe"}, \
                    {(char*)"zesty", (char*)"picante"}, \
                    {(char*)"zillion", (char*)"billón"}, \
                    {(char*)"zucchini", (char*)"calabacín"}, \
                    {(char*)"zeppelin", (char*)"dirigible"}}, \
                    19, false};


static Dictionary dictionary = {
    {&section_a, &section_b, &section_c, &section_d, &section_e,
    &section_f, &section_g, &section_h, &section_i, &section_j,
    &section_k, &section_l, &section_m, &section_n, &section_o,
    &section_p, &section_q, &section_r, &section_s, &section_t,
    &section_u, &section_v, &section_w, &section_x, &section_y,
    &section_z}, 
    0,
    0,
    {0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0,\
    0, 0, 0, 0, 0, 0}
};



unsigned int str_pos = 0;
unsigned int col_pos = 0;

char input_string[41];
unsigned int input_index = 0;
bool shift_flag = 0;

unsigned int time = 0;
int last_w = 0;
unsigned long last_s = 0;
static unsigned short int s[256];    
static unsigned short int s_i, s_j; 
static bool inited;


/* OS functional */

char* kitoa(int number)
{
    if (number == 0) return (char*)"0";
    int i = 0, j = 0;
    static char *reverse;
    while (number != 0)
    {
        reverse[i] = (char)(number % 10 + '0');
        number /= 10;
        i++;
    }
    reverse[i] = '\0';
    i--;
    static char result[10];
    for (; i >= 0; j++, i--)
    {
        result[j] = reverse[i];
    }
    result[j] = '\0';
    return result;
}

int kstrlen(char *arg_array)
{
  int len = 0;
  for (; *(arg_array + len); len++);
  return len;
}

char* kstrcat(char *dest, char *src)
{
    static char ptr[81];
    unsigned i,j;
    for (i = 0; dest[i] != '\0'; i++) ptr[i] = dest[i];
    for (j = 0; src[j] != '\0'; j++) ptr[i+j] = src[j];
    ptr[i + j] = '\0';
    return ptr;
}

char kstrcmp(char *string1, char *string2) 
{
    char* str1 = string1, *str2 = string2;
    while ('\0' != *str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str2 - *str1;
}

char kstrncmp(char * string1, char * string2, int n)
{
    for (int i = 0; i < n; ++i)
	{
		if (string1[i] != string2[i]) {
			return 0;
		}
	}
	return 1;
}

char* get_argument(char* command)
{
    bool space_flag = 0;
    static char buffer[41];
    int j = 0;
    for (int i = 0; *(command + i); i++)
    {
        if (!space_flag && *(command + i) == ' ')
        {
            space_flag = 1;
        }
        else if (space_flag && *(command + i) == ' ')
        {
            return (char*)"";
        }
        else if (space_flag)
        {
            *(buffer + j) = *(command + i);
            j++;
        }
    }
    if (!buffer) return (char*)"";
    *(buffer + j) = '\0';
    return buffer;
}

void swap(char* a[2], char* b[2]) {
    char* temp[2] = { a[0], a[1] };
    a[0] = b[0];
    a[1] = b[1];
    b[0] = temp[0];
    b[1] = temp[1];
}

int partition(char* arr[][2], int low, int high) {
    char* pivot = arr[high][0]; 
    int i = (low - 1); 

    for (int j = low; j < high; j++) {
        if (kstrcmp(arr[j][0], pivot) < 0) {
            i++;
            swap(arr[i], arr[j]); 
        }
    }
    swap(arr[i + 1], arr[high]); 
    return (i + 1);
}

void quickSort(char* arr[][2], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    }
}

void sortSection(DictSection *section) {
    quickSort(section->words_pairs, 0, section->word_count - 1);
}

char* bin_search(char* word, DictSection* section)
{
    int left = 0;
    int right = section->word_count - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        int cmp_result = kstrcmp(word, section->words_pairs[mid][0]);

        if (cmp_result == 0) 
        {
            return section->words_pairs[mid][1];
        }
        else if (cmp_result < 0) 
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    return (char*)""; 
}

static inline void swap_byte (unsigned short int *a, unsigned short int *b) 
{
  unsigned short int t = *a;
  *a = *b;
  *b = t;
}
 
void random_init (unsigned seed)
{
  unsigned short int *seedp = (unsigned short int *) &seed;
  int i;
  unsigned short int j;

  if (inited)
    return;

  for (i = 0; i < 256; i++) 
    s[i] = i;
  for (i = j = 0; i < 256; i++) 
    {
      j += s[i] + seedp[i % sizeof(seed)];
      swap_byte (s + i, s + j);
    }

  s_i = s_j = 0;
  inited = true;
}
 
void random_bytes (void *buf_, unsigned int size) 
{
  unsigned short int *buf;

  if (!inited)
    random_init (0);

  for (buf = (unsigned short int *)buf_; size-- > 0; buf++)
    {
      unsigned short int s_k;
      
      s_i++;
      s_j += s[s_i];
      swap_byte (s + s_i, s + s_j);

      s_k = s[s_i] + s[s_j];
      *buf = s[s_k];
    }
}

unsigned long random_ulong (void) 
{
  static unsigned long ul;
  random_bytes (&ul, sizeof(ul));
  return ul;
}

static inline void outb(unsigned short port, unsigned char data) 
{
	asm volatile ("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

static inline void outw(unsigned int port, unsigned int data)
{
	asm volatile ("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

static inline unsigned char inb(unsigned short port) 
{
	unsigned char data;
	asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
	return data;
}

void video_up()
{
    unsigned char* video_buf = (unsigned char*)VIDEO_BUF_PTR;
    for(unsigned int i = 0; i < VIDEO_LEN - 1; i++) 
    {
        for (unsigned int j = 0; j < VIDEO_WIDTH*2 - 1; j++)
        {
            video_buf[(i * VIDEO_WIDTH * 2) + j] = video_buf[((i+1) * VIDEO_WIDTH * 2) + j];
        }               
    }
    for(unsigned int i = 0; i < VIDEO_WIDTH*2; i += 2) {
        video_buf[((VIDEO_LEN - 1) * VIDEO_WIDTH * 2) + i]  = ' ';
        video_buf[((VIDEO_LEN - 1) * VIDEO_WIDTH * 2) + i + 1] = WHITE;
    }
    str_pos = VIDEO_LEN - 1;
}

void cursor_moveto(unsigned int strnum, unsigned int pos)
{
	unsigned short new_pos = (strnum * VIDEO_WIDTH) + pos;
	outb(CURSOR_PORT, 0x0F);
	outb(CURSOR_PORT + 1, (unsigned char)(new_pos & 0xFF));
	outb(CURSOR_PORT, 0x0E);
	outb(CURSOR_PORT + 1, (unsigned char)((new_pos >> 8) & 0xFF));
}

static inline void out_char(const char sym, unsigned int strnum)
{
    unsigned char* video_buf = (unsigned char*)VIDEO_BUF_PTR;
    if (strnum >= VIDEO_LEN)
    {
        video_up();
        strnum = str_pos;
    }
    video_buf += VIDEO_WIDTH*strnum*2 + col_pos;
    video_buf[0] = (unsigned char) sym;  
    video_buf[1] = WHITE;
    video_buf += 2;
    col_pos += 2;
    cursor_moveto(str_pos, col_pos/2);
}

static inline void out_str(const char* ptr, unsigned int strnum)
{
    unsigned char* video_buf = (unsigned char*)VIDEO_BUF_PTR;
    if (strnum >= VIDEO_LEN)
    {
        video_up();
        strnum = str_pos;
    }
    video_buf += VIDEO_WIDTH*strnum*2;
    while (*ptr)
    {
        if ((unsigned char)*ptr > 128)
        {
            unsigned char ch = (unsigned char)*ptr;
            int unicode_char = 0;
            if ((ch & 0xE0) == 0xC0) { 
                unsigned char next = *(ptr + 1);
                if (next) {
                    unicode_char = ((ch & 0x1F) << 6) | (next & 0x3F);
                }
            }
            switch (unicode_char) {
            case 0x00E1: // 'á'
                video_buf[0] = 133; 
                break;
            case 0x00E9: // 'é'
                video_buf[0] = 138; 
                break;
            case 0x00ED: // 'í'
                video_buf[0] = 141; 
                break;
            case 0x00F3: // 'ó'
                video_buf[0] = 149; 
                break;
            case 0x00FA: // 'ú'
                video_buf[0] = 151; 
                break;
            case 0x00F1: // 'ñ'
                video_buf[0] = 164; 
                break;
            case 0x00FC: // 'ü'
                video_buf[0] = 129; 
                break;
            default:
                video_buf[0] = '?'; 
                break;
            }
            video_buf[1] = WHITE;
            video_buf += 2; 
            ptr += 2; 

        }
        else
        {
            video_buf[0] = (unsigned char) *ptr;  
            video_buf[1] = WHITE;
            video_buf += 2; 
            ptr++; 
        }
    }
    col_pos = 0;
    str_pos++;
    cursor_moveto(str_pos, col_pos/2);
}

static inline void info()
{
    out_str("_____________INFO___________________", str_pos);
    out_str("System: DictOS (English -> Spanish).", str_pos);
    out_str("Author: Butina A.A.", str_pos);
    out_str("Assembler translator: YASM (Intel syntax).", str_pos);
    out_str("Kernel compiler: gcc.", str_pos);
    out_str("Loaded words starting with letters: ", str_pos);
    col_pos = 0;
    for (unsigned short i = 0; i < 26; i++)
    {
        if (dictionary.letters_array[i] != 0) out_char(('a' + i), str_pos);
        else out_char('_', str_pos);
    }
    str_pos++;
    col_pos = 0;
    cursor_moveto(str_pos, col_pos);
}

void dictinfo()
{
    out_str("_________DICTINFO___________________", str_pos);
    out_str("Dictionary: en -> es", str_pos);
    char* out_1 = kstrcat((char*)"Number of words: ", kitoa(dictionary.words_count));
    out_str(out_1, str_pos);
    char* out_2 = kstrcat((char*)"Number of loaded words: ", kitoa(dictionary.active_count));
    out_str(out_2, str_pos);
    cursor_moveto(str_pos, 0);
}

void translate() 
{
    out_str((char*)"_________TRANSLATE__________________", str_pos);
    char* arg = get_argument(input_string);
    if (!kstrcmp(arg, (char*)"") || arg[0] < 'a' || arg[0] > 'z')
    {
        out_str("ERROR: Invalid argument. Try again!", str_pos);
    }
    else
    {
        int letter_num = (unsigned)(arg[0] - 'a');
        if (dictionary.letters_array[letter_num])
        {
            char* translation = bin_search(arg, dictionary.sections[letter_num]);
            if (*(translation) == '\0')
            {
                char* result = kstrcat(kstrcat((char*)"Error: A word '", arg), (char*)"' is unknown");//?
                out_str(result, str_pos);
            }
            else
            {
                out_str(translation, str_pos);
            }
        }
        else
        {
            char* result = kstrcat(kstrcat((char*)"Error: A word '", arg), (char*)"' is unknown");
            out_str(result, str_pos);
        }
    }
}

void wordstat()
{
    out_str("_________WORDSTAT___________________", str_pos);
    char* arg = get_argument(input_string);
    if (!kstrcmp(arg, (char*)"") || kstrlen(arg)>1 || arg[0] < 65 || \
    (arg[0] > 90 && arg[0] < 97) || arg[0] > 122)
    {
        out_str("ERROR: Invalid argument. Try again!", str_pos);
        return;
    }
    else
    {
        int letter_num = (arg[0] < 'a') ? (unsigned)(arg[0] - 'A') : (unsigned)(arg[0] - 'a');
        if (dictionary.letters_array[letter_num])
        {
            char* result = kstrcat(kstrcat((char*)"Letter '", arg), (char*)"': ");
            result = kstrcat(result, kitoa(dictionary.sections[letter_num]->word_count));
            result = kstrcat(result, (char*)" words loaded.");
            out_str(result, str_pos);
            return;
        }
        else
        {
            char* out = kstrcat(kstrcat((char*)"Letter '", arg), (char*)"': 0 words loaded.");
            out_str(out, str_pos);
            return;
        }
    }
}

static inline void shutdown()
{
	outw(0x604, 0x2000);
}

void anyword()
{
    out_str((char*)"__________ANYWORD___________________", str_pos);
    char* arg = get_argument(input_string);
    if (kstrlen(arg) > 1)
    {
        out_str("ERROR: Invalid argument. Try again!", str_pos);
        return;
    }
    else if (kstrlen(arg) == 1 && (arg[0] < 'a' || arg[0] > 'z'))
    {
        out_str("ERROR: Invalid argument. Try again!", str_pos);
        return;
    }
    else if (!kstrlen(arg))
    {
        random_init(time);
        unsigned long section_number = random_ulong() % 26;
        while (section_number == last_s || dictionary.letters_array[section_number] == 0)section_number = random_ulong() % 26;
        last_s = section_number;
        DictSection *curr = dictionary.sections[section_number];
        unsigned long word_number = random_ulong() % curr->word_count;
        while (word_number == last_w) word_number = random_ulong() % curr->word_count;
        last_w = word_number;
        char* word_pair = kstrcat(kstrcat(curr->words_pairs[word_number][0], (char*)": "), curr->words_pairs[word_number][1]);
        out_str(word_pair, str_pos);
        return;
    }
    else
    {
        int letter_num = (unsigned)(arg[0] - 'a');
        if (!dictionary.letters_array[letter_num])
        {
            out_str("ERROR: Letter is not load. Try again!", str_pos);
            return;
        }
        DictSection *curr = dictionary.sections[letter_num];
        random_init(time);
        int word_number = (int)random_ulong() % (int)curr->word_count;
        while (word_number == last_w) word_number = random_ulong() % curr->word_count;
        last_w = word_number;
        char* word_pair = kstrcat(kstrcat(curr->words_pairs[word_number][0], (char*)": "), curr->words_pairs[word_number][1]);
        out_str(word_pair, str_pos);
        return;
    }
}

static inline void wrong_command()
{
    out_str((char*)"Error: No command found.", str_pos);
}

void enter()
{
    if (!kstrcmp(input_string, (char*)"info")) info();
	else if (!kstrcmp(input_string, (char*)"dictinfo")) dictinfo(); 
	else if (kstrncmp(input_string, (char*)"translate", 9)) translate(); 
	else if (kstrncmp(input_string, (char*)"wordstat", 8)) wordstat();
	else if (kstrncmp(input_string, (char*)"anyword ", 8)) anyword(); 
	else if (!kstrcmp(input_string, (char*)"anyword")) anyword(); 
	else if (!kstrcmp(input_string, (char*)"shutdown")) shutdown();
    else wrong_command();

    for (int i = 40; i>=0; i--)
    {
        *(input_string+i) = '\0';
    }
    input_index = 0;
    col_pos = 0;
    cursor_moveto(str_pos, 1);
}

static inline void wait_enter(unsigned int strnum)
{
    time++;
    if (time == 4294967295) {
        time = 0;
    }
    unsigned char* video_buf = (unsigned char*)VIDEO_BUF_PTR;
    if (strnum >= VIDEO_LEN)
    {
        video_up();
        strnum = str_pos;
    }
    video_buf += VIDEO_WIDTH*strnum*2;
    video_buf[0] = '#';  
    video_buf[1] = WHITE;
    video_buf += 2;
}

char code_to_char(unsigned char code, bool flag)
{
    static char char_table[] = { \
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, \
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0, 0, 0, \
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',	'l', 0, 0, 0, 0, \
        0, 'z', 'x', 'c', 'v', 'b', 'n', 'm', 0, 0, '/', 0, 0, 0, ' ', 0, 0
	};
    static char shift_table[] = { \
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, \
        0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0, 0, 0, \
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0, \
        0, 0, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, '?', 0, 0, 0, ' ', 0, 0
    };
    return (flag)? shift_table[code] : char_table[code];
}

void read_code(unsigned char code)
{
    char sym = '\0';
    if (code < 128)
    {
        if (code == SHIFT)
        {
            shift_flag = 1;
            return;
        }
        else if (code == BACKSPACE)
        {
            if (input_index != 0)
            {
                unsigned char* video_buf = (unsigned char*)VIDEO_BUF_PTR;
                col_pos -= 2;
                video_buf += VIDEO_WIDTH*str_pos*2 + col_pos;
                video_buf[0] = ' ';
                video_buf[1] = WHITE;
                cursor_moveto(str_pos, col_pos/2);
                input_index--;
                input_string[input_index] = '\0';
            }
        }
        else if (code == ENTER)
        {
            col_pos = 0;
            str_pos++;
            input_string[input_index] = '\0';
            input_index++;
            enter();
        }
        else if (input_index < 40)
        {
            sym = code_to_char(code, shift_flag);
            if (sym != '\0')
            {
                if (input_index == 0) col_pos = 2;
                out_char(sym, str_pos);
                input_string[input_index] = sym;
                input_index++;
            }
        }
    }
    else if (code == SHIFT + OFFSET)
    {
        shift_flag = 0;
        return;
    }
}

void process_key()
{
    if (inb(0x64) & 0x01)
    {
        unsigned char scan_code = inb(0x60);
        read_code(scan_code);
    }
}

void intr_keyboard_handler()
{
    asm("pusha");
    process_key();
    outb(PIC1_PORT, 0x20);
    asm("popa; leave; iret");
}

void intr_default_handler()
{
	asm("pusha");
	asm("popa; leave; iret");
}

void intr_reg_handler(int num, unsigned short segm_sel, unsigned short flags, intr_handler handler)
{
	unsigned int handler_address = (unsigned int)handler;
	g_idt[num].base_lo = (unsigned short)(handler_address & 0xFFFF);
	g_idt[num].segm_sel = segm_sel;
	g_idt[num].always0 = 0;
	g_idt[num].flags = flags;
	g_idt[num].base_hi = (unsigned short)(handler_address >> 16);
}

void intr_init()
{
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);
    for (int i = 0; i < idt_count; i++)
    {
        // segm_sel=0x8, P=1, DPL=0, Type=Intr
        intr_reg_handler(i, GDT_CS, 0x80 | IDT_TYPE_INTR, intr_default_handler);
    }
}

void keyboard_intr_init()
{
    intr_reg_handler(0x09, GDT_CS, 0x80 | IDT_TYPE_INTR, intr_keyboard_handler);
    outb(PIC1_PORT + 1, 0xFF ^ 0x02);
}

void intr_enable()
{
    asm("sti");
}

void intr_disable()
{
    asm("cli");
}

void intr_start()
{
    int idt_count = sizeof(g_idt) / sizeof(g_idt[0]);
	g_idtp.base = (unsigned int)(&g_idt[0]);
	g_idtp.limit = (sizeof(struct idt_entry) * idt_count) - 1;
	asm("lidt %0" : : "m" (g_idtp));
}



extern "C" int kmain()
{
    char* ptr = (char*)(0x9000);
    for (unsigned short i = 0; i < 26; i++)
    {
        dictionary.letters_array[i] = (int)(*(ptr+i));
        sortSection(dictionary.sections[i]);
        if (*(dictionary.letters_array + i) == 1)
        {
            dictionary.sections[i]->allow_flag = 1;
            dictionary.active_count += dictionary.sections[i]->word_count;

            time++;
            if (time == 4294967295) {
                time = 0;
            }
        }
        dictionary.words_count += dictionary.sections[i]->word_count;
    }
    for (int i = 40; i>=0; i--)
    {
        *(input_string+i) = '\0';
    }

    const char* hello_string = "Welcome to DictOS!";
    out_str(hello_string, str_pos);
    cursor_moveto(str_pos, col_pos/2 + 1);

    intr_disable();
    intr_init();
    keyboard_intr_init();
    intr_start();
    intr_enable();

	while (1)
	{
        wait_enter(str_pos);
		asm("hlt");
	}

    return 0;
}
