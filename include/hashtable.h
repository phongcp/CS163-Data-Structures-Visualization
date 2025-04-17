#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "var.h"
#include "manager.h"
#include "tinyfiledialogs.h"

#define EMPTY -1 // Giá trị mặc định cho ô trống trong bảng băm
#define DELETED -2 // Giá trị mặc định cho ô đã xóa trong bảng băm


// HashTable class definition
class HashTable {
private:

    int CELLS_PER_ROW = 10;    // Mỗi hàng có 10 ô

    // Thông số vẽ ô (hình tròn)
    float FULL_RADIUS = 30.0f;
    float CELL_SPACING_X = 90.0f;
    float CELL_SPACING_Y = 90.0f;
    float OFFSET_X = 450.0f;
    float OFFSET_Y = 150.0f;   // Dành cho bảng, phía trên dành cho slider và nhập liệu

    // Thời gian animation
    float COLLISION_DURATION = 0.5f;
    float GROWING_DURATION = 0.5f;
    float INITIAL_ANIM_DURATION = 0.5f; // Thời gian animation ban đầu của tất cả các ô

    // Các trạng thái của quá trình chèn số mới
    enum InsertState { WAITING_INPUT, COLLISION_CHECK, GROWING_ANIMATION, INITIAL_HASH_TALBE};
    InsertState currentInsertState = WAITING_INPUT;    

    struct NodeHashTable{
        int data;
        std::string txt;
        Vector2 position;
        Color cellColor, borderCellColor;
        float radius;
        Font font;
        float fontSize;
        bool isUpdate = false;

        NodeHashTable(){};

        NodeHashTable(int value){
            data = value;
        }
        
        NodeHashTable(int value, Vector2 pos, Color color1, Color color2, float r, Font f, float fSize, bool update = false){
            data = value; // Giá trị của ô
            txt = std::to_string(value);
            position = pos; // Vị trí của ô
            cellColor = color1; // Màu sắc của ô
            borderCellColor = color2; // Màu viền của ô
            radius = r; // Bán kính của ô
            font = f; // Phông chữ
            fontSize = fSize; // Kích thước phông chữ
            isUpdate = update; // Trạng thái cập nhật
        }
        
        void draw(float opacity = 1.0f, float rad = 0) {
            // DrawCircle(position.x, position.y, ((rad == 0) ? radius : rad) + 2, Fade(borderCellColor, opacity)); // Vẽ viền hình tròn
            DrawCircle(position.x, position.y, ((rad == 0) ? radius : rad) + 2, isLightMode ? borderCellColor : ConvertToDarkMode(borderCellColor)); // Vẽ viền hình tròn
            DrawCircle(position.x, position.y, (rad == 0) ? radius : rad, Fade(isLightMode ? cellColor : ConvertToDarkMode(cellColor), opacity)); // Vẽ hình tròn

            // DrawCircle(position.x, position.y, (rad == 0) ? radius : rad, Fade(cellColor, opacity)); // Vẽ hình tròn
            // DrawCircleLines(position.x, position.y, radius, Fade(borderCellColor, opacity)); // Vẽ viền hình tròn
            if(data == -2){
                txt = "DEL"; // Nếu ô đã xóa, hiển thị chữ DEL
                DrawTextEx(font, txt.c_str(), { position.x - MeasureText(txt.c_str(), fontSize) / 2.0f, position.y - fontSize / 2.0f }, fontSize, 1.0f, Fade(isLightMode ? BLACK : WHITE, opacity)); // Vẽ DEL trong hình tròn
            }
            else if (data != -1) {
                DrawTextEx(font, txt.c_str(), { position.x - MeasureText(txt.c_str(), fontSize) / 2.0f, position.y - fontSize / 2.0f }, fontSize, 1.0f, Fade(isLightMode ? BLACK : WHITE, opacity)); // Vẽ chữ trong hình tròn
            }
        }
    };

    struct Snapshot {
        std::vector<NodeHashTable> table;     // Trạng thái của bảng (mỗi ô chứa số hoặc -1 nếu trống)
        InsertState state;          // Trạng thái chèn tại thời điểm đó
        int checkIndex;             // Chỉ số ô đang kiểm tra (nếu có)
        float insertTimer;          // Thời gian animation hiện tại
        int insertingNumber;        // Số đang được chèn (nếu có)
        const char** code = nullptr; // Mã lệnh tương ứng với trạng thái hiện tại
        int codeLine1 = -1, codeLine2 = -1;

        Snapshot(std::vector<NodeHashTable> newTable, const char** newCode = nullptr, int newCodeLine1 = -1, int newCodeLine2 = -1){
            table = newTable;
            code = newCode; // Mã lệnh tương ứng với trạng thái hiện tại
            codeLine1 = newCodeLine1; // Dòng đầu tiên của mã lệnh
            codeLine2 = newCodeLine2; // Dòng thứ hai của mã lệnh
        }
    };


    // Pseudocode for hash function
    const char* insertValueCode[6] = {
        "if K+1 == N, return\n",
        "id = value % size\n",
        "while (hash[id] != EMPTY)\n",
        "    id = (id + 1) % size\n",
        "hash[id] = value, K++\n",
        ""
    };

    const char* searchValueCode[6] = {
        "id = value % size\n",
        "while (hash[id] != EMPTY)\n",
        "    if (hash[id] == value) return id\n",
        "    id = (id + 1) % size\n",
        "return NOT_FOUND\n",
        ""
    };

    const char* deleteValueCode[6] = {
        "id = value % size\n",
        "while (hash[id] != EMPTY)\n",
        "  if (hash[id] == value) ",
        "    hash[id] = DELETED,K++,return\n",
        "  id = (id + 1) % size\n",
        "return NOT_FOUND\n",
    };

    enum ANIMATION_STATE{PAUSE, PLAY, REPLAY};
    ANIMATION_STATE animationState; // Trạng thái animation hiện tại

    int lineCount;
    Rectangle togglePseudocodeButton;
    bool isPseudocodeVisible;
    void drawPseudocode(const char**, int, int);
    void updatePseudocodeOn();

    int capacity;                // Maximum size of the table
    int size;                    // Current number of elements in the table
    bool inputActive;    // Flag to check if input is active
    int inputType;       // Type of input (0: None, 1: Create, 2: Search, 3: Insert, 4: Delete)    

    std::vector<Snapshot> Animation; // Lịch sử animation
    std::vector<NodeHashTable> hashTable; // The hash table
    float timeAnimation = 0.0f; // Thời gian hiện tại của animation
    int curAnimation = 0;   // Chỉ số animation hiện tại

    std::vector<button> remote;

    ButtonText addButton; // Button to add a new key-value pair
    ButtonText searchButton; // Button to search for a value by key
    ButtonText deleteButton; // Button to delete a key-value pair
    ButtonText updateButton;
    ButtonText createButton; // Button to create a random hash table
    ButtonText fileButton; // Button to handle file operations
    ButtonText clearButton; // Button to clear the hash table

    InputTextBox createNTextBox; 
    InputTextBox createKTextBox;
    InputTextBox searchTextBox;
    InputTextBox addTextBox;
    InputTextBox updateUTextBox, updateVTextBox;
    InputTextBox deleteTextBox;

    Rectangle sizeInputBox; // Rectangle for size input box
public:
    HashTable();
    Vector2 generatePosition(int index);
    void init();
    void createRandomTable(int, int);
    void insertValueToHashTable(int);
    void insert(int);
    void search(int);
    void remove(int);
    void update(int, int);
    void clear();
    void loadFile();
    void handleEvents();
    void handleRemote();
    void drawHashTable(Snapshot, float);
    void draw();
    void drawButtons();
    ~HashTable();
};

#endif // HASHTABLE_H