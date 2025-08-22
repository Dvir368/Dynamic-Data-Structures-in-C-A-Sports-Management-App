//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define EventData "EventData.txt"
#define SportsmanData "SportsmanData.txt"
#define MAX_FNAME 10
#define MAX_LNAME 15
#define MAX_CLUB_NAME 30
#define MAX_LINE_LENGTH 100

typedef struct {
    char* p2title;
    char* p2location;
    int year;
} event;

typedef struct {
    int id;
    char Fname[MAX_FNAME + 1];
    char Lname[MAX_LNAME + 1];
    char* p2club;
    int gen;
    event* p2events;
    int Nevents;
} sportsman;

// הצהרה על פונקציות
FILE* openAndCheckEmpty();
void* allocateMemory();
void FreeAllSportsmen();
int getCheckedInput();
char* allocateAndCopyString();
void updatefileSportman();
void updatefileEvent();
int countLines();
void FromFile2Sportsman();
void FromFile2Events();
int addSportsman();
int addEvent();
int printEvents();
int countEvent();
void BestClub();
int CheckSameEvent();
void printSortedEvents();
void deleteEvent();
void NewClub();
void printMenu();


// הפונקציה הראשית של התוכנית
int main()
{
    int sportsmenCount = countLines(SportsmanData); // ספירת השורות בקובץ נתוני הספורטאים
    if (sportsmenCount <= 0) // בדיקה האם נקראו נתונים והאם כמות השורות חוקית
    {
        printf("Failed to read sportsman data.\n");
        return 1; // יציאה מהתוכנית במקרה של כישלון בקריאת הנתונים
    }

    sportsman* sportsmen = malloc(sportsmenCount * sizeof(sportsman)); // הקצאת זיכרון עבור מערך הספורטאים
    if (!sportsmen) // בדיקת הצלחת הקצאת הזיכרון למערך הספורטאים
    {
        printf("Memory allocation failed for sportsmen.\n");
        return 1; // יציאה מהתוכנית במקרה של כישלון בהקצאת הזיכרון
    }
    FromFile2Sportsman(SportsmanData, sportsmen, sportsmenCount); // קריאה לפונקציה לקריאת הנתונים מקובץ הספורטאים והכנסתם למערך הספורטאים
    FromFile2Events(EventData, sportsmen, sportsmenCount); // קריאה לפונקציה לקריאת הנתונים מקובץ האירועים והכנסתם למערך הספורטאים
    int choice;
    while (1) // רץ בלולאה אינסופית עד לבחירת האופציה ליציאה
    {
        printMenu(); // הצגת התפריט למשתמש
        scanf("%d", &choice); // קולט את הבחירה מהמשתמש
        getchar(); // לנטרל את האינטר שנותר בקלט

        switch (choice)
        {
        case 1: // הוספת ספורטאי חדש
        {
            // בדיקה אם ההוספה הצליחה והדפסת הודעה מתאימה
            if (addSportsman(&sportsmen, &sportsmenCount))
                printf("Sportsman added successfully.\n");
            else
                printf("Failed to add sportsman.\n");
            break;
        }
        case 2: // הוספת אירוע לספורטאי קיים
        {
            int id;
            printf("Enter sportsman ID: "); // בקשת ה-ID מהמשתמש
            scanf("%d", &id); // קליטת ה-ID
            getchar(); // ניטרול Enter

            // בדיקה אם ההוספה הצליחה והדפסת הודעה מתאימה
            if (addEvent(id, sportsmen, sportsmenCount))
                printf("Event added successfully.\n");
            else
                printf("Failed to add event.\n");
            break;
        }
        case 3: // הדפסת אירועים של ספורטאי מסוים
        {
            char lastname[MAX_LNAME];
            printf("Please enter the last name: "); // בקשת שם המשפחה מהמשתמש
            scanf("%s", lastname); // קליטת שם המשפחה
            getchar(); // ניטרול Enter
            int result = printEvents(lastname, sportsmen, sportsmenCount); // ביצוע הפונקציה וקבלת התוצאה
            // הדפסת הודעה בהתאם לתוצאה
            if (result == 0)
                printf("No events available for this sportsman.\n");
            else if (result == -1)
                printf("This sportsman does not exist.\n");
            break;
        }
        case 4: // ספירת כמות הספורטאים באירוע מסוים
        {
            char eventName[MAX_LINE_LENGTH];
            int year;
            printf("Enter event name: "); // בקשת שם האירוע מהמשתמש
            fgets(eventName, MAX_LINE_LENGTH, stdin); // קליטת שם האירוע
            eventName[strcspn(eventName, "\n")] = 0; // הסרת תו ירידת שורה מסוף המחרוזת
            printf("Enter event year: "); // בקשת שנת האירוע מהמשתמש
            scanf("%d", &year); // קליטת שנת האירוע
            getchar(); // ניטרול Enter
            printf("Number of sportsmen in event: %d\n", countEvent(eventName, year, sportsmen, sportsmenCount)); // הדפסת כמות הספורטאים באירוע
            break;
        }
        case 5: // מציאת המועדון עם מספר האירועים הגבוה ביותר
            BestClub(sportsmen, sportsmenCount);
            break;
        case 6: // בדיקה אם ספורטאי השתתף באותם אירועים עם ספורטאים אחרים
        {
            int id;
            printf("Enter sportsman ID: "); // בקשת ה-ID מהמשתמש
            scanf("%d", &id); // קליטת ה-ID
            getchar(); // ניטרול Enter
            CheckSameEvent(id, sportsmen, sportsmenCount); // בדיקה אם קיימים אירועים משותפים והדפסת הודעה מתאימה
            break;
        }
        case 7: // הדפסת כל האירועים של מועדון מסוים, ממוינים לפי שנה
        {
            char clubName[MAX_CLUB_NAME];
            printf("Enter club name: "); // בקשת שם המועדון מהמשתמש
            fgets(clubName, MAX_CLUB_NAME, stdin); // קליטת שם המועדון
            clubName[strcspn(clubName, "\n")] = 0; // הסרת תו ירידת שורה מסוף המחרוזת
            printSortedEvents(clubName, sportsmen, sportsmenCount); // ביצוע הפונקציה
            break;
        }
        case 8: // מחיקת אירוע
        {
            char eventName[MAX_LINE_LENGTH];
            int year;
            printf("Enter event name: "); // בקשת שם האירוע מהמשתמש
            fgets(eventName, MAX_LINE_LENGTH, stdin); // קליטת שם האירוע
            eventName[strcspn(eventName, "\n")] = 0; // הסרת תו ירידת שורה מסוף המחרוזת
            printf("Enter event year: "); // בקשת שנת האירוע מהמשתמש
            scanf("%d", &year); // קליטת שנת האירוע
            getchar(); // ניטרול Enter
            deleteEvent(eventName, year, sportsmen, sportsmenCount); // ביצוע הפונקציה
            printf("Event deleted if existed.\n"); // הדפסת הודעה
            break;
        }
        case 9: // יצירת קובץ עבור מועדון חדש עם אירועים משותפים
        {
            char C1[MAX_CLUB_NAME], C2[MAX_CLUB_NAME];
            printf("Enter first club name: "); // בקשת שם המועדון הראשון מהמשתמש
            fgets(C1, MAX_CLUB_NAME, stdin); // קליטת שם המועדון הראשון
            C1[strcspn(C1, "\n")] = 0; // הסרת תו ירידת שורה מסוף המחרוזת
            printf("Enter second club name: "); // בקשת שם המועדון השני מהמשתמש
            fgets(C2, MAX_CLUB_NAME, stdin); // קליטת שם המועדון השני
            C2[strcspn(C2, "\n")] = 0; // הסרת תו ירידת שורה מסוף המחרוזת
            NewClub(C1, C2, sportsmen, sportsmenCount); // ביצוע הפונקציה
            printf("New club file created.\n"); // הדפסת הודעה
            break;
        }
        case 0:
            // יציאה מהתוכנית
            printf("Exiting...\n");
            updatefileEvent(EventData, sportsmen, sportsmenCount); // עידכון האירועים לקובץ
            updatefileSportman(SportsmanData, sportsmen, sportsmenCount); // עידכון של הספורטאים לקובץ
            FreeAllSportsmen(sportsmen, sportsmenCount); // שחרור זיכרון של כל הספורטאים והאירועים שלהם

            return 0; // יציאה מהפונקציה ובכך מהתוכנית
        default: // במקרה של בחירה לא תקינה
            printf("Invalid choice, please try again.\n");
        }
    }
}

///****פונקציות עזר****

// פתיחת קובץ ובדיקה אם הוא ריק
FILE* openAndCheckEmpty(const char* filename)
{
    FILE* file = fopen(filename, "r"); // פתיחת הקובץ לקריאה
    if (!file) // בדיקה אם נכשלה פתיחת הקובץ
    {
        printf("Error opening file: %s\n", filename);
        return NULL; // במקרה של כישלון בפתיחת הקובץ החזרת NULL
    }
    fseek(file, 0, SEEK_END); // העברת הקורסור לסוף הקובץ
    if (ftell(file) == 0) // בדיקה אם הקובץ ריק
    {
        printf("The file is empty\n");
        fclose(file); // סגירת הקובץ
        return NULL; // במקרה של קובץ ריק החזרת NULL
    }
    rewind(file); // החזרת הסמן לתחילת הקובץ
    return file; // החזרת הקובץ במקרה של פתיחה תקינה
}

// פונקציה זו מקבלת את גודל הזיכרון הרצוי להקצאה ומחזירה מצביע אליו
void* allocateMemory(size_t size)
{
    void* memory = malloc(size); // הקצאת זיכרון בגודל המבוקש
    if (!memory) // בדיקה אם ההקצאה נכשלה
    {
        perror("Failed to allocate memory");
        exit(1); // יציאה מהתוכנית במקרה של כישלון בהקצאת הזיכרון
    }
    return memory; // החזרת המצביע לזיכרון המוקצה במקרה של הצלחה בהקצאה
}

// שחרור זיכרון של כל הספורטאים והאירועים שלהם
void FreeAllSportsmen(sportsman* sportmen, int size)
{
    if (!sportmen) // בדיקה אם המערך ריק או לא מוקצה, וחזרה מידית אם כן
        return;

    for (int i = 0; i < size; i++) // ריצה על כל אחד מהאלמנטים במערך
    {
        free(sportmen[i].p2club); // שחרור הזיכרון שהוקצה למועדון של האלמנט הנוכחי
        for (int j = 0; j < sportmen[i].Nevents; j++) // ריצה על כל האירועים של הספורטאי הנוכחי
        {
            free(sportmen[i].p2events[j].p2title); // שחרור הזיכרון שהוקצה לכותרת של כל אחד מהאירועים
            free(sportmen[i].p2events[j].p2location); // שחרור הזיכרון שהוקצה למיקום של כל אחד מהאירועים
        }
        free(sportmen[i].p2events); // לאחר שחרור כל המידע הפנימי של האירועים, שחרור הזיכרון שהוקצה למערך האירועים עצמו
    }
    free(sportmen); // לבסוף, שחרור הזיכרון שהוקצה למערך הספורטאים עצמו
}

// קליטת מחרוזת מהמשתמש עם בדיקת תקינות
int getCheckedInput(const char* message, char* target, int maxLength)
{
    printf("%s", message);
    if (fgets(target, maxLength, stdin) == NULL) // קריאת קלט מהמשתמש אם נכשלה, הדפסת הודעת שגיאה
    {
        printf("Failed to read input.\n");
        return 0;
    }

    target[strcspn(target, "\n")] = 0; // הסרת תו ירידת שורה אם קיים
    if (strlen(target) == 0) // בדיקה אם הקלט ריק לאחר הסרת תו ירידת השורה
    {
        printf("Input cannot be empty.\n");
        return 0;
    }
    return 1; // חזרה עם אחד, משמעות הדבר שהקלט תקין
}

// הקצאת זיכרון והעתקת מחרוזת
char* allocateAndCopyString(const char* source)
{
    if (source == NULL) // בדיקה אם הכתובת שהתקבלה ריקה. אם כן, חוזרים עם כתובת ריקה
        return NULL;
    char* copy = (char*)malloc(strlen(source) + 1); // הקצאת זיכרון עבור העתק של המחרוזת, כולל מקום לתו סיום המחרוזת
    if (copy) // בדיקה אם ההקצאה הצליחה
        strcpy(copy, source); // העתקת התוכן של המחרוזת המקורית לזיכרון שהוקצה
    else
        printf("Failed to allocate memory for string copy.\n"); // הדפסת הודעת שגיאה אם ההקצאה נכשלה
    return copy; // החזרת הכתובת של המחרוזת המועתקת
}

// מקבלת את המערך וגודלו לאחר השינויים בתפריט ומעדכנת את הנתונים של הספורטאים לקובץ
void updatefileSportman(const char* filename, const sportsman* sportsmen, int size) {
    // פתיחת קובץ לכתיבה
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    // כתיבת כותרת העמודות בקובץ
    fprintf(file, "format:sportsman_id;first_name;last_name;club_name;gender\n");

    // עבור כל ספורטאי במערך
    for (int i = 0; i < size; ++i) // כתיבת נתוני הספורטאים לקובץ
    {
        fprintf(file, "%d;%s;%s;%s;%d", sportsmen[i].id, sportsmen[i].Fname, sportsmen[i].Lname, sportsmen[i].p2club, sportsmen[i].gen);
        if (i < size - 1)
            fprintf(file, "\n");
    }

    // סגירת הקובץ
    fclose(file);
}

// מקבלת את המערך וגודלו לאחר השינויים בתפריט ומעדכנת את האירועים של הספורטאים לקובץ
void updatefileEvent(const char* filename, const sportsman* sportsmen, int size) {
    FILE* file = fopen(filename, "w"); // פתיחת קובץ לכתיבה
    assert(file != NULL); // בדיקה שהפתיחה של הקובץ הצליחה
    // כתיבת השורה הראשונה בקובץ שזה הפורמט
    fprintf(file, "format:sportsman_id,event_name,location,year\n");

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < sportsmen[i].Nevents; ++j) {
            // בדיקה שכל אחד מהשדות אינו NULL לפני הכתיבה לקובץ
            if (sportsmen[i].p2events[j].p2title != NULL && sportsmen[i].p2events[j].p2location != NULL) {
                fprintf(file, "%d,%s,%s,%d\n", sportsmen[i].id, sportsmen[i].p2events[j].p2title, sportsmen[i].p2events[j].p2location, sportsmen[i].p2events[j].year);
            }
        }
    }
    fclose(file); // סגירת הקובץ
}

///****עד לפה פונקציות עזר****

// ספירת השורות בקובץ
int countLines(const char* filename)
{
    FILE* file = openAndCheckEmpty(filename); // פתיחת קובץ ובדיקה אם ריק.
    if (!file) // אם הקובץ לא נפתח.
        return -1; // חזרה עם שגיאה.
    int count = 0; // מונה שורות.
    int ch; // משתנה לקריאת תו.
    while ((ch = fgetc(file)) != EOF) // קריאת תווים עד סוף הקובץ.
    {
        if (ch == '\n') // אם מצא תו ירידת שורה.
            count++; // תוסיף למונה.
    }
    fclose(file); // סגירת קובץ.
    return count; // החזרת מספר השורות.
}

// קריאת נתוני ספורטאים מקובץ והזנתם למערך
void FromFile2Sportsman(const char* filename, sportsman* array, int size)
{
    FILE* file = openAndCheckEmpty(filename); // פתיחת ובדיקת קובץ
    if (!file)
        return; // חזרה אם קובץ לא נפתח

    char line[MAX_LINE_LENGTH]; // מערך לשמירת שורה
    char clubBuffer[MAX_LINE_LENGTH]; // מערך זמני לשמות מועדון
    int count = 0; // מונה ספורטאים

    fgets(line, sizeof(line), file); // קריאת שורה ראשונה, התעלמות
    while (count < size && fscanf(file, "%d;%10[^;];%15[^;];%30[^;];%d", &array[count].id, array[count].Fname, array[count].Lname, clubBuffer, &array[count].gen) == 5) // קריאה והזנה מקובץ למערך
    {
        if (clubBuffer != NULL && *clubBuffer == ' ') // בודק אם התו הראשון בשם המועדון הוא רווח ואם כן מוחק את הרווח על ידי התקדמות עם מצביע
        {
            char* p2buffer = clubBuffer;
            while (*p2buffer != '\0') // חיפוש לתו האחרון במחרוזת
            {
                *p2buffer = *(p2buffer + 1); // העברת התו הבא על ידי העתקה
                p2buffer++; // התקדמות במחרוזת
            }

            // החזרת המצביע p2buffer לתחילת המחרוזת החדשה ל clubBuffer
            p2buffer = clubBuffer;
        }

        array[count].p2club = allocateAndCopyString(clubBuffer); // הקצאת זיכרון לשם מועדון
        if (!array[count].p2club)
        {
            printf("Memory allocation failed for club name.\n"); // שגיאה אם הקצאה נכשלה
            break; // יציאה מהלולאה בכישלון
        }
        array[count].p2events = NULL; // אתחול רשימת אירועים
        array[count].Nevents = 0; // אתחול מונה אירועים
        count++; // הגדלת מונה

    }

    fclose(file); // סגירת קובץ
}

// קריאת נתוני אירועים מקובץ והוספתם לספורטאים במערך
void FromFile2Events(const char* filename, sportsman* sportsmen, int size)
{
    FILE* file = openAndCheckEmpty(filename); // פתיחת ובדיקת קובץ
    if (!file)
        return; // חזרה אם קובץ לא נפתח

    int id, year;
    char line[MAX_LINE_LENGTH];
    char titleBuffer[MAX_LINE_LENGTH], locationBuffer[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // קריאת שורה ראשונה, התעלמות
    while (fscanf(file, "%d,%[^,],%[^,],%d", &id, titleBuffer, locationBuffer, &year) == 4) // קריאת נתוני אירוע מקובץ
    {
        for (int i = 0; i < size; i++) // חיפוש הספורטאי המתאים במערך
        {
            if (sportsmen[i].id == id) // אם מצא תואם
            {
                int newCount = sportsmen[i].Nevents + 1; // עדכון מונה אירועים
                event* tempEvents = realloc(sportsmen[i].p2events, newCount * sizeof(event)); // הרחבת מערך אירועים
                if (!tempEvents)
                {
                    printf("Memory allocation failed for new event.\n"); // אם הקצאה נכשלה
                    fclose(file);
                    return;
                }
                sportsmen[i].p2events = tempEvents; // עדכון מערך אירועים
                sportsmen[i].p2events[newCount - 1].year = year; // הזנת שנה

                // הקצאת זיכרון והעתקה לפרטי אירוע
                sportsmen[i].p2events[newCount - 1].p2title = allocateAndCopyString(titleBuffer);
                sportsmen[i].p2events[newCount - 1].p2location = allocateAndCopyString(locationBuffer);

                if (!sportsmen[i].p2events[newCount - 1].p2title || !sportsmen[i].p2events[newCount - 1].p2location)
                {
                    printf("Memory allocation failed for event details.\n"); // אם הקצאה לפרטים נכשלה

                    for (int j = 0; j < newCount - 1; j++) // שחרור זיכרון בכישלון
                    {
                        free(sportsmen[i].p2events[j].p2title);
                        free(sportsmen[i].p2events[j].p2location);
                    }
                    free(sportsmen[i].p2events); // שחרור מערך אירועים
                    fclose(file); // סגירת קובץ
                    return;
                }

                sportsmen[i].Nevents = newCount; // עדכון מונה אירועים
                break; // סיום לולאה לאחר מציאת הספורטאי
            }
        }
    }

    fclose(file); // סגירת קובץ
}

// הוספת ספורטאי חדש למערך הספורטאים
int addSportsman(sportsman** sportsmen, int* size)
{
    sportsman newSportsman; // יצירת משתנה חדש לספורטאי
    char club[MAX_CLUB_NAME + 1]; // מערך לשמירת שם מועדון
    int result; // משתנה לתוצאת פעולה

    // קליטת ID עם בדיקת תקינות
    printf("Enter ID (positive number): ");
    result = scanf("%d", &newSportsman.id); // קליטת ID
    while ((result != 1 || newSportsman.id <= 0) || getchar() != '\n') // בדיקת תקינות קלט
    {
        printf("Invalid ID. Please enter a positive number: "); // הדפסת שגיאה בקלט לא תקין
        result = scanf("%d", &newSportsman.id); // ניסיון קריאה מחדש
    }


    for (int i = 0; i < *size; i++) // לולאת בדיקה עבור כל ספורטאי במערך
    {
        if ((*sportsmen)[i].id == newSportsman.id) // אם ID כבר קיים
        {
            printf("A sportsman with this ID already exists.\n"); // הדפסת שגיאה
            return 0; // יציאה מהפונקציה
        }
    }

    if (!getCheckedInput("Enter first name: ", newSportsman.Fname, MAX_FNAME + 1)) // בדיקה וקריאה של שם פרטי
        return 0; // יציאה אם הקלט לא תקין

    if (!getCheckedInput("Enter last name: ", newSportsman.Lname, MAX_LNAME + 1)) // בדיקה וקריאה של שם משפחה
        return 0; // יציאה אם הקלט לא תקין

    if (!getCheckedInput("Enter club name: ", club, MAX_CLUB_NAME + 1)) // בדיקה וקריאה של שם מועדון
        return 0; // יציאה אם הקלט לא תקין
    newSportsman.p2club = allocateAndCopyString(club); // הקצאת זיכרון והעתקת שם מועדון

    printf("Enter gender (0 -> male, 1 -> female): "); // בקשת מגדר
    result = scanf("%d", &newSportsman.gen); // קריאת מגדר
    while ((result != 1 || (newSportsman.gen != 0 && newSportsman.gen != 1)) || getchar() != '\n') // בדיקת תקינות קלט
    {
        printf("Invalid gender. Please enter 0 for female or 1 for male: "); // הדפסת שגיאה אם הקלט לא תקין
        result = scanf("%d", &newSportsman.gen); // ניסיון קריאה מחדש
    }

    newSportsman.p2events = NULL; // אתחול מערך אירועים ל-NULL
    newSportsman.Nevents = 0; // אתחול מונה אירועים


    *size += 1; // הרחבת מערך הספורטאים
    *sportsmen = realloc(*sportsmen, (*size) * sizeof(sportsman)); // הרחבת זיכרון מערך
    if (*sportsmen == NULL)
    {
        printf("Failed to reallocate memory for sportsmen array.\n"); // הדפסת שגיאה אם הקצאה נכשלה
        free(newSportsman.p2club); // שחרור זיכרון שם מועדון
        return 0; // יציאה מהפונקציה
    }

    (*sportsmen)[*size - 1] = newSportsman; // הכנסת הספורטאי החדש למערך

    return 1; // החזרת ערך הצלחה
}

// הוספת אירוע חדש לספורטאי קיים במערך
int addEvent(int id, sportsman* sportsmen, int size)
{
    for (int i = 0; i < size; i++) // לולאה על כל הספורטאים
    {
        if (sportsmen[i].id == id) // אם מצא את הספורטאי בעל ID תקין
        {
            char title[MAX_LINE_LENGTH], location[MAX_LINE_LENGTH]; // מערכים לשם האירוע ומיקומו
            int year; // משתנה לשנה


            if (!getCheckedInput("Enter event title: ", title, MAX_LINE_LENGTH)) // קליטת שם האירוע ובדיקת תקינות
                return 0; // יציאה במקרה של קלט לא תקין

            if (!getCheckedInput("Enter event location: ", location, MAX_LINE_LENGTH)) // קליטת מיקום האירוע ובדיקת תקינות
                return 0; // יציאה במקרה של קלט לא תקין

            printf("Enter event year: ");
            if (scanf("%d", &year) != 1 || year < 1800 || year > 2100 || getchar() != '\n') // קליטת שנת האירוע ובדיקת תקינות
            {
                printf("Invalid year. Please enter a year between 1800 and 2100.\n"); // הדפסת שגיאה
                return 0; // יציאה במקרה של קלט לא תקין
            }

            for (int j = 0; j < sportsmen[i].Nevents; j++) // בדיקה אם האירוע כבר קיים
            {
                if (strcmp(sportsmen[i].p2events[j].p2title, title) == 0 && strcmp(sportsmen[i].p2events[j].p2location, location) == 0 && sportsmen[i].p2events[j].year == year) // בודק אם האירוע הנוכחי זהה לאירוע שנרצה להוסיף לספורטאי, על פי שם, מיקום ושנה
                {
                    printf("Event already exists.\n"); // הדפסת שגיאה אם האירוע כבר קיים
                    return 0; // יציאה מהפונקציה
                }
            }

            // הרחבת מערך האירועים
            int newCount = sportsmen[i].Nevents + 1; // עדכון מונה האירועים
            sportsmen[i].p2events = realloc(sportsmen[i].p2events, newCount * sizeof(event)); // הקצאת זיכרון נוסף
            if (!sportsmen[i].p2events)
            {
                printf("Memory allocation failed.\n"); // הדפסת שגיאה אם ההקצאה נכשלה
                return 0; // יציאה מהפונקציה
            }

            // הקצאת זיכרון והעתקת שם האירוע והמיקום
            sportsmen[i].p2events[newCount - 1].p2title = allocateAndCopyString(title); // העתקת שם האירוע
            sportsmen[i].p2events[newCount - 1].p2location = allocateAndCopyString(location); // העתקת מיקום האירוע
            if (!sportsmen[i].p2events[newCount - 1].p2title || !sportsmen[i].p2events[newCount - 1].p2location)
            {
                printf("Memory allocation failed for event details.\n"); // הדפסת שגיאה אם ההקצאה נכשלה
                return 0; // יציאה מהפונקציה
            }

            sportsmen[i].p2events[newCount - 1].year = year; // הזנת שנת האירוע
            sportsmen[i].Nevents = newCount; // עדכון מספר האירועים

            return 1; // הוספת האירוע בהצלחה
        }
    }

    printf("Sportsman ID %d not found.\n", id); // הדפסת שגיאה אם ה-ID לא נמצא
    return 0; // יציאה מהפונקציה
}

// הדפסת אירועים לפי שם משפחה של ספורטאי
int printEvents(const char* lastname, const sportsman* sportsmen, int size)
{
    int found = 0; // משתנה זה משמש לסימון האם נמצא ספורטאי עם שם המשפחה הנתון

    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsmen[i].Lname, lastname) == 0)
        {
            if (sportsmen[i].Nevents == 0)
            {
                printf("No events available for %s.\n", lastname); // הדפסת הודעת שגיאה כאשר אין אירועים זמינים
                return 0; // חזרה מיידית אם אין אירועים, מונע הדפסה כפולה
            }
            else
            {
                found = 1; // מצאנו את הספורטאי
                printf("Events for %s:\n", lastname); // הדפסת הכותרת לרשימת האירועים
                for (int j = 0; j < sportsmen[i].Nevents; j++)
                    printf("Event %d: %s, Location: %s, Year: %d\n", j + 1, sportsmen[i].p2events[j].p2title, sportsmen[i].p2events[j].p2location, sportsmen[i].p2events[j].year); // הדפסת פרטי כל אירוע
                return 1; // חזרה מיידית לאחר הדפסת האירועים
            }
        }
    }

    if (!found)
        printf("Sportsman with last name %s not found.\n", lastname); // הדפסה זו תתבצע רק אם לא מצאנו את הספורטאי בכלל

    return -1; // חזרה עם -1 אם הספורטאי לא נמצא
}

// פונקציה זו מחזירה את מספר האירועים עם שם ושנת אירוע נתונים, בהתאם לספורטאים הקיימים במערך
int countEvent(const char* E, int Y, const sportsman* sportsmen, int size)
{
    int count = 0; // משתנה זה משמש לספירת מספר האירועים התואמים

    for (int i = 0; i < size; i++) // עבור כל ספורטאי במערך
    {
        for (int j = 0; j < sportsmen[i].Nevents; j++) // עבור כל אירוע של הספורטאי הנוכחי
        {
            if (strcmp(sportsmen[i].p2events[j].p2title, E) == 0 && sportsmen[i].p2events[j].year == Y)
            {
                count++; // ספירת האירוע אם שם האירוע והשנה תואמים
                break; // לא נדרש להמשיך לבדוק את האירועים של הספורטאי הנוכחי
            }
        }
    }

    return count; // החזרת מספר האירועים התואמים
}

// מחפשת את המועדון עם מספר האירועים המרבי במצטבר מבין כל הספורטאים ומדפיסה את שמו ואת כמות האירועים
void BestClub(sportsman* sportsmen, int size)
{
    if (size == 0) // בדיקה אם אין נתונים לספורטאים
    {
        printf("No sportsmen data available.\n"); // הדפסת הודעת שגיאה
        return; // חזרה מיידית מהפונקציה
    }

    // יצירת מילון למפת מועדונים לספירת אירועים
    typedef struct {
        char clubName[MAX_CLUB_NAME + 1]; // שם המועדון
        int eventCount; // מספר האירועים
    } ClubEventCount;

    ClubEventCount* clubEventCounts = (ClubEventCount*)allocateMemory(size * sizeof(ClubEventCount)); // הקצאת זיכרון למילון
    int clubCount = 0; // מונה מועדונים

    // מעבר על כל הספורטאים וספירת אירועים למועדון
    for (int i = 0; i < size; i++)
    {
        int found = 0; // משתנה לסימון אם המועדון נמצא במילון כבר
        for (int j = 0; j < clubCount; j++)
        {
            if (strcmp(sportsmen[i].p2club, clubEventCounts[j].clubName) == 0)
            {
                clubEventCounts[j].eventCount += sportsmen[i].Nevents; // ספירת אירועים למועדון קיים במילון
                found = 1; // המועדון נמצא במילון
                break;
            }
        }
        if (!found) // אם המועדון לא נמצא במילון
        {
            strcpy(clubEventCounts[clubCount].clubName, sportsmen[i].p2club); // הוספת המועדון למילון
            clubEventCounts[clubCount].eventCount = sportsmen[i].Nevents; // ספירת אירועים למועדון חדש
            clubCount++; // עדכון מספר המועדונים
        }
    }

    int maxEvents = 0; // מספר האירועים המרבי
    char* bestClub = NULL; // שם המועדון עם מספר האירועים המרבי
    for (int i = 0; i < clubCount; i++)
    {
        if (clubEventCounts[i].eventCount > maxEvents)
        {
            maxEvents = clubEventCounts[i].eventCount; // עדכון מספר האירועים המרבי
            bestClub = clubEventCounts[i].clubName; // עדכון שם המועדון עם מספר האירועים המרבי
        }
    }

    if (bestClub != NULL)
        printf("Best Club: %s with %d events.\n", bestClub, maxEvents); // הדפסת המועדון עם הכי הרבה אירועים
    else
        printf("No club found.\n"); // הדפסת הודעת שגיאה אם לא נמצא מועדון
    free(clubEventCounts); // שחרור הזיכרון שהוקצה למילון
}

// פונקציה לבדיקת אירועים משותפים לספורטאי עם מספר זיהוי נתון במערכת
int CheckSameEvent(int I, sportsman* sportsmen, int size)
{
    int foundSharedEvent = 0; // משתנה לצורך זיהוי האם נמצאו אירועים משותפים או לא

    // חיפוש הספורטאי עם המזהה I
    for (int i = 0; i < size; i++)
    {
        if (sportsmen[i].id == I)
        {
            for (int j = 0; j < sportsmen[i].Nevents; j++) // סריקת כל אחד מהאירועים של הספורטאי
            {
                event currentEvent = sportsmen[i].p2events[j]; // אירוע נוכחי של הספורטאי


                for (int k = 0; k < size; k++) // בדיקת האירוע נגד אירועי הספורטאים האחרים
                {

                    if (sportsmen[k].id != I) // דילוג על הספורטאי עצמו
                    {
                        for (int l = 0; l < sportsmen[k].Nevents; l++) // רץ על כל האירועים של הספורטאי המתאים במערך
                        {
                            if (strcmp(currentEvent.p2title, sportsmen[k].p2events[l].p2title) == 0 && currentEvent.year == sportsmen[k].p2events[l].year) // בודק אם האירוע הנוכחי של הספורטאי הנבחר תואם לאירוע של ספורטאי אחר באותה שנה
                            {
                                if (!foundSharedEvent)
                                {
                                    printf("Shared events found for %d:\n", I);
                                    foundSharedEvent = 1;
                                }
                                printf("Event: %s, Year: %d with %s %s\n", currentEvent.p2title, currentEvent.year, sportsmen[k].Fname, sportsmen[k].Lname);
                            }
                        }
                    }
                }
            }
            break; // סיום החיפוש באירועים של הספורטאי הנבחר
        }
    }

    if (!foundSharedEvent)
        printf("No shared events found for %d.\n", I);

    return foundSharedEvent;
}

// ממיינת ומדפיסה את כל האירועים לפי שנה מספורטאים במועדון נתון, כל אירוע פעם אחת
void printSortedEvents(const char* clubName, sportsman* sportsmen, int size)
{
    // ספירת כמות האירועים במועדון הנתון
    int totalEvents = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsmen[i].p2club, clubName) == 0) // בדיקה האם הספורטאי משתייך למועדון וספירת האירועים שלו
            totalEvents += sportsmen[i].Nevents;
    }

    // בדיקה אם לא נמצאו אירועים והדפסת הודעה מתאימה
    if (totalEvents == 0)
    {
        printf("No events or club '%s' does not exist.\n", clubName);
        return;
    }

    // הקצאת זיכרון לאחסון כל האירועים מהמועדון הנתון
    event* events = malloc(totalEvents * sizeof(event));
    int count = 0;

    // איסוף כל האירועים מכל הספורטאים במועדון הנתון
    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsmen[i].p2club, clubName) == 0)
        {
            // איסוף האירועים לתוך המערך החדש
            for (int j = 0; j < sportsmen[i].Nevents; j++)
                events[count++] = sportsmen[i].p2events[j];

        }
    }

    // מיון האירועים לפי שנה
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (events[j].year > events[j + 1].year)
            {
                // החלפת מקומות במערך בהתאם לשנת האירוע
                event temp = events[j];
                events[j] = events[j + 1];
                events[j + 1] = temp;
            }
        }
    }

    // הדפסת האירועים לאחר המיון, עם התייחסות לאירועים זהים
    for (int i = 0; i < count; i++)
        if (i == 0 || events[i].year != events[i - 1].year || strcmp(events[i].p2title, events[i - 1].p2title) != 0)
            printf("%d)Event: %s, Year: %d\n", i + 1, events[i].p2title, events[i].year);

    free(events); // שחרור הזיכרון שנקצה למערך האירועים
}

// פונקציה זו מוחקת אירוע ממערך האירועים של ספורטאים על פי שם האירוע ושנתו
void deleteEvent(const char* E, int Y, sportsman* sportsmen, int size)
{

    for (int i = 0; i < size; i++) // עבור כל ספורטאי במערך
    {
        for (int j = 0; j < sportsmen[i].Nevents; j++) // עבור כל אירוע של הספורטאי
        {
            if (strcmp(sportsmen[i].p2events[j].p2title, E) == 0 && sportsmen[i].p2events[j].year == Y) // בדיקה האם האירוע תואם את הפרמטרים שהתקבלו
            {
                // מצאנו את האירוע למחיקה
                // הזזת כל האירועים הנותרים כדי למחוק את האירוע הנוכחי
                for (int k = j; k < sportsmen[i].Nevents - 1; k++)
                    sportsmen[i].p2events[k] = sportsmen[i].p2events[k + 1];

                sportsmen[i].Nevents -= 1; // עדכון מספר האירועים של הספורטאי
                break; // יציאה מהלולאה אחרי מחיקת האירוע
            }
        }
    }
}

// מדפיסה לקובץ את כל האירועים שהספורטאים משני מועדונים ספציפיים השתתפו בהם, ללא חזרות, בהתבסס על רשימת ספורטאים וגודלה
void NewClub(const char* C1, const char* C2, sportsman* sportsmen, int size)
{
    FILE* file = fopen("Club.txt", "w");
    assert(file != NULL); // בדיקה שהפתיחה של הקובץ הצליחה

    // הגדרת מבנה לשמירת אירועים ייחודיים
    typedef struct UniqueEvent {
        char* title; // כותרת אירוע ייחודי
        int printed; // סמן הדפסת אירוע
    } UniqueEvent;

    UniqueEvent* uniqueEvents = malloc(size * sizeof(UniqueEvent) * 10); // הקצאת זיכרון למבנה נתונים לכל אירוע ייחודי
    if (!uniqueEvents)
    {
        printf("Memory allocation failed for unique events.\n"); // בדיקת הצלחת הקצאת זיכרון
        fclose(file);
        return;
    }
    int uniqueCount = 0; // מונה כמות האירועים הייחודיים

    for (int i = 0; i < size; i++)
    {
        if (strcmp(sportsmen[i].p2club, C1) == 0 || strcmp(sportsmen[i].p2club, C2) == 0) // בדיקת שני המועדונים
        {
            for (int j = 0; j < sportsmen[i].Nevents; j++) // לולאה על כל האירועים של כל ספורטאי
            {
                int exists = 0; // משתנה לבדיקת האם האירוע כבר קיים
                for (int k = 0; k < uniqueCount; k++) {
                    if (strcmp(sportsmen[i].p2events[j].p2title, uniqueEvents[k].title) == 0)
                    {
                        exists = 1;
                        break;
                    }
                }
                if (!exists) // אם האירוע אינו קיים כבר ברשימה
                {
                    uniqueEvents[uniqueCount].title = allocateAndCopyString(sportsmen[i].p2events[j].p2title); // העתקת שם האירוע
                    uniqueEvents[uniqueCount].printed = 0; // איתור אירוע חדש שיש להדפיס
                    uniqueCount++; // עדכון כמות האירועים הייחודיים
                }
            }
        }
    }

    for (int i = 0; i < uniqueCount; i++)
    {
        if (!uniqueEvents[i].printed) // הדפסת האירועים הייחודיים
        {
            fprintf(file, "%s\n", uniqueEvents[i].title);
            uniqueEvents[i].printed = 1; // סימון האירוע כהודפס
        }
    }

    fclose(file); // סגירת קובץ
    for (int i = 0; i < uniqueCount; i++)
        free(uniqueEvents[i].title); // שחרור זיכרון שנמצא בשימוש

    free(uniqueEvents); // שחרור זיכרון שנמצא בשימוש
}

// הצגת תפריט המשתמש וביצוע פעולות לפי בחירת המשתמש
void printMenu()
{
    // הדפסת תפריט האפשרויות למשתמש
    printf("\n\t MAIN MENU:"
        "\n 1. Add sportsman to array"
        "\n 2. Add event to sportsman"
        "\n 3. Print events of a sportsman"
        "\n 4. Count sportsmen in an event"
        "\n 5. Find the best club"
        "\n 6. Check if a sportsman participated in the same event with others"
        "\n 7. Print all events of a club sorted by year"
        "\n 8. Delete an event"
        "\n 9. Create a new club"
        "\n 0. Exit"
        "\n Your choice: ");
}