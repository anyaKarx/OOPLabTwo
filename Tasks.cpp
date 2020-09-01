#include <exception>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include<string>
#include <array>
#include <vector>
#include <functional>
using namespace std;
//TODO: если планируешь работать на плюсах, то разбивай проект на файлы как принято в плюсах -
// с одной стороны, используешь сложные техники для обучения, с другой стороны - весь код в одном cpp-файле...
namespace details
{
    bool IsNotLetter(uint16_t symbol)
    {
		//TODO: магические числа надо выносить в именованные константы
        bool isNumber = symbol < 1040,
            isSpecSymbol = symbol > 1103;
        return isNumber || isSpecSymbol;
    }

    template<typename PointerType>
    void WriteAddress(PointerType *ptrOne, PointerType *ptrTwo)
    {
        wcout << L"Адрес в первом указателе: " << ptrOne
            << L". Адрес во втором указателе: " << ptrTwo << endl;
    }

    void Sort(double* values, int count)
    {
        double swap;
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count; j++)
            {
                if (values[i] < values[j])
                {
                    swap = values[i];
                    values[i] = values[j];
                    values[j] = swap;
                }
            }
        }
    }
}

template<typename valueType, typename isTheValueValidFunction> //TODO: зачем далее пустая строка? Так принято? Код должен быть оформлен единообразно

valueType GetValidValueFromUser(wstring message, isTheValueValidFunction isTheValueValid)
{
    valueType value = 0;
    bool isInvalid = true;

    while (isInvalid)
    {
        wcout << message;
        wcin >> value;
        bool isStreamFail = wcin.fail();
        isInvalid = isStreamFail || !isTheValueValid(value) || wcin.peek() != L'\n';
        if (isInvalid)
        {
            if (isStreamFail)
                wcin.clear();

            wcout << L"Значение недопустимо, попробуйте еще раз:" << endl;
        }
        wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    }
	//TODO: очистка экрана внутри часто используемых функций может мешать разработчику сделать правильную логику пользовательского интерфейса на верхнем уровне. Лучше вызывать очистку экрана в более высокоуровневых функциях, так у разработчика будет гибкость в управлении содержанием экрана
    system("cls");
    return value;
}


wstring GetValidStringFromUser()
{
    wstring input;
    bool isCorrect = false;
    uint16_t spaceUnicode = 32;
    uint16_t dashUnicode = 45;
    while (!isCorrect)
    {
        getline(wcin, input);
        isCorrect = true;
        for (size_t i = 0; i < input.length(); i++)
        {
            if (details::IsNotLetter((unsigned int)input[i]) 
                && ((unsigned int)input[i] != spaceUnicode 
                && ((unsigned int)input[i] != dashUnicode)))
            {
                wcout << L" Ввод должен быть на русском языке" << endl;
                isCorrect = false;
                break;
            }
        }
    }

    return move(input);
};
//TODO: не более одной пустой строки


//2.2.1.1.


void Sort(double* values, int count)
{
    return count >= 0
        ? details::Sort(values, count)
        : throw exception("count < 0");
}

void DemoSort(int count)
{
    try
    {
        double* values = new double[count] {100.0, 249.0, 12.0, 45.0, 23.5};
        Sort(values, count);
        for (int i = 0; i < count; i++)
        {
            wcout << values[i] << endl;
        }
        delete[] values;
    }
    catch (exception e)
    {
        wcout << "Exception catched!" << endl;
    }
}

//2.2.2.1.

enum class Colors //TODO: зачем слово class?
{
    YELLOW, //TODO: именование всеми заглавными буквами, а следующие перечисления написаны как обычные слова - оформить код единообразно
    GREEN,
    BLUE,
    BROWN,
    WHITE,
    RED,
    ORANGE,
    PINK,
    BLACK,
    END
};

struct Rectangle
{
    float Length;
    float Width;
    Colors Color;
}; //TODO: лишняя пустая строка


wostream& operator<<(wostream& os, const Rectangle& rec)
{
    return os << L" Прямоугольник имеет размеры " << rec.Length << L"x"
        << rec.Width;
}

struct Flight
{
    wstring DeparturePoint;
    wstring Destination;
    uint16_t FlightTimeInMinutes = 0;
}; //TODO: нет пустой строки
wostream& operator<<(wostream& os, const Flight& flight)
{
    return os << L" Рейс " << flight.DeparturePoint << L" - " << flight.Destination
        << L" находится в полете " << flight.FlightTimeInMinutes << L" минут.";
}

enum class Genres //TODO: зачем слово class
{
    HORROR,
    DOCUMENTARY,
    ANIMATION,
    THRILLER,
    DRAMA,
    COMEDY,
    ADVENTURE,
    END
};

wstring GenreNameForConsole(Genres genre)
{
    array <wstring, (size_t)Genres::END> NameGenres{L" ужасы ", L" документальное кино",
     L" анимация ", L" триллер ", L" драма ",L" комедия ", L" приключения"};
    
    return NameGenres[(int)genre];
}

Genres ReadGenre()
{
    wcout << L"Наберите номер необходимого вам жанра: " << endl
        << L" 0 - ужасы,\n 1 - документальное кино,\n 2 - анимация," << endl
        << L" 3 - триллер,\n 4 - драма,\n 5 - комедия,\n 6 - приключения" << endl;
    uint16_t userChoice = GetValidValueFromUser<uint16_t>(L" > ",
        [](auto const& val) {return val >= 0 && val < (uint16_t)Genres::END; });
    Genres genre = (Genres)userChoice;
    return genre;
}

struct Film
{
    wstring Name;
    uint16_t DurationInMinutes = 0;
    uint16_t YearOfIssue = 0;
    Genres genre; //TODO: именование
    double Rating = 0;
};

wostream& operator<<(wostream& os, const Film& movie)
{
    return os << L" Фильм " << movie.Name << L" жанра"<< GenreNameForConsole(movie.genre)
        << L"вышел в " << movie.YearOfIssue << L" году. Его длительность " 
        << movie.DurationInMinutes << L" минут, а рейтинг " << movie.Rating << L" баллов.";
}

struct TimeStamp
{
    uint16_t Hour = 0;
    uint16_t Min = 0;
    uint16_t Sec = 0;
};

wostream& operator<<(wostream& os, const TimeStamp& time)
{
    return os << L" Зафиксированное время: " << time.Hour << L":"
        << time.Min << L":" << time.Sec << L".";
}

//2.2.2.2.
struct Contact
{
    wstring Name;
    wstring Surname;
    char PhoneNumber[10];
};

struct Song
{
    wstring Name;
    wstring Performer;
    wstring Album;
    TimeStamp Duration;
};
//TODO: почему Options?
//TODO: зачем class?
enum class GradesOptions
{
    //PASS = 0,поскольку оценка зачтено не имеет бального эквивалента
    PASS,
    FAIL = 2,
    SATISFACTORY,
    GOOD,
    EXCELLENT
};

struct Subject
{
    wstring Discipline;
    uint8_t NumberOfHours;
    wstring TeacherSurname;
    GradesOptions Grade;
};


//2.2.3.1
Rectangle DemoRectangle(float length, float width, Colors color)
{
    return Rectangle { length, width, color };
}

Flight DemoFlight(wstring departurePoint, wstring destination, uint16_t flightTimeInMinutes)
{
    return Flight{ departurePoint, destination, flightTimeInMinutes };
}

Film DemoMovie(wstring name, uint16_t duration, uint16_t yearOfIssue, Genres genre, double rating)
{//TODO: Film или Movie? Вызываю метод DemoMovie, а получаю Film. Вредит читаемости кода
    return Film{ name, duration, yearOfIssue, genre, rating };
}

TimeStamp DemoTime(uint16_t hour, uint16_t min, uint16_t sec)
{
    return TimeStamp{ hour, min, sec };
}

//2.2.3.2

auto GetInfoOfRectangleFromUser()
{
    auto customRectangle = make_unique< Rectangle>();
    customRectangle->Length = GetValidValueFromUser<float>(L"Введите длину прямоугольника (положительное число):\n > ",
        [](auto const& val) {return val > 0; });
    customRectangle->Width = GetValidValueFromUser<float>(L"Введите ширину прямоугольника (положительное число):\n > ",
        [](auto const& val) {return val > 0; });
    return customRectangle;
}

auto GetInfoOfFlightfromUser()
{
    auto customFlight = make_unique< Flight>();
    wcout << L" Введите пункт вылета (на русском языке)" << endl << " > ";
    customFlight->DeparturePoint = GetValidStringFromUser();
    wcout << L" Введите пункт назначения (на руссклм языке)" << endl << "> ";
    customFlight->Destination = GetValidStringFromUser();
    customFlight->FlightTimeInMinutes = GetValidValueFromUser<uint16_t>(L" Введите длительность полета в минутах: \n > ",
        [](auto const& val) {return val > 0; });
    return customFlight;
}
//TODO: именование
auto GetInfoOfFilmFromUser()
{
    auto customMovie = make_unique<Film>();
    wcout << L" Введите название фильма на русском" << endl << L" > ";
    customMovie->Name = GetValidStringFromUser();
    customMovie->YearOfIssue = GetValidValueFromUser<uint16_t>(L"Введите год выхода фильма:\n > ",
        [](auto const& val) {return val > 1887 && val < 2021; });
    customMovie->DurationInMinutes = GetValidValueFromUser<uint16_t>(L"Введите длительность фильма в минутах:\n > ",
        [](auto const& val) {return val > 0 && val < 999; });
    customMovie->Rating = GetValidValueFromUser<double>(L"Введите рейтинг фильма:\n > ",
        [](auto const& val) {return val >= 0 && val <= 10.0; });
    customMovie->genre = ReadGenre();
    return customMovie;
}

auto GetTimeFromUser()
{
    auto customTime = make_unique<TimeStamp>();
    customTime->Hour = GetValidValueFromUser<uint16_t>(L"Введите часы (от 1 до 24):\n > ",
        [](auto const& val) {return val > 0 && val < 25; });
    customTime->Min = GetValidValueFromUser<uint16_t>(L"Введите минуты :\n > ",
        [](auto const& val) {return val >= 0 && val < 60; });
    customTime->Sec = GetValidValueFromUser<uint16_t>(L"Введите секунды :\n > ",
        [](auto const& val) {return val >= 0 && val < 60; });
    return customTime;
}

//2.2.3.3.

void MakeAndOutputArrayOfRectangles()
{
    array<Rectangle, 3> rectangles = { DemoRectangle(2.5, 7, Colors::BROWN),
                                      DemoRectangle(10, 15, Colors::BLACK),
                                      DemoRectangle(15, 3.9, Colors::PINK) };
    copy(rectangles.begin(), rectangles.end(), ostream_iterator<Rectangle, wchar_t>(wcout, L"\n"));
}

void MakeAndOutputArrayFlight()
{
    array<Flight, 5> flights = { DemoFlight(L" Москва", L"Санкт-петербург", 45),
                                 DemoFlight(L" Москва", L"Анапа", 368),
                                 DemoFlight(L" Томск", L"Пермь", 489),
                                 DemoFlight(L" Челябинск", L"Пенза", 285),
                                 DemoFlight(L" Москва", L"Новосибирск", 181) };
    copy(flights.begin(), flights.end(), ostream_iterator<Flight, wchar_t>(wcout, L"\n"));
}

void MakeAndOutputArrayFilm()
{
    array<Film, 4> movies = { DemoMovie(L"Мстители", 210, 2010, Genres::ADVENTURE, 7.9),
                              DemoMovie(L"Реинкарнация", 145, 2018, Genres::HORROR, 7.5),
                              DemoMovie(L"Джиа", 210, 2003, Genres::DRAMA, 8.2),
                              DemoMovie(L"Кошмар на улице вязов", 115, 1983, Genres::HORROR, 10) };
    copy(movies.begin(), movies.end(), ostream_iterator<Film, wchar_t>(wcout, L"\n"));
}

void MakeAndOutputArrayTime()
{
    array<TimeStamp, 3> times = { DemoTime(12, 53, 05),
                                  DemoTime(7, 26, 39),
                                  DemoTime(23, 0, 59) };
    copy(times.begin(), times.end(), ostream_iterator<TimeStamp, wchar_t>(wcout, L"\n"));
}

//2.2.4

void PrintAddressAndInfoOfRectangle()
{
    Rectangle rectangleOne = DemoRectangle(2.5, 7, Colors::BROWN);
    Rectangle* structureRectanglePointer = &rectangleOne;
    wcout <<  *structureRectanglePointer << endl;
    structureRectanglePointer->Length = 5;
    structureRectanglePointer->Width = 10.2;
    wcout << L" Новый " << *structureRectanglePointer << endl;
    Rectangle* structureRectanglePointerTwo = &rectangleOne;
    details::WriteAddress(structureRectanglePointer, structureRectanglePointerTwo);
}

void PrintAddressAndInfoOfFlight()
{
    Flight FlightOne = DemoFlight(L"Томск", L"Москва", 224);
    Flight* structureFlightPointer = &FlightOne;
    wcout << *structureFlightPointer << endl;
    structureFlightPointer->DeparturePoint = L"Москва";
    structureFlightPointer->Destination = L"Санкт-петербург";
    structureFlightPointer->FlightTimeInMinutes = 45;
    wcout << L" Новый " << *structureFlightPointer << endl;
    Flight* structureFlightPointerTwo = &FlightOne;
    details::WriteAddress(structureFlightPointer, structureFlightPointerTwo);
}

void PrintAddressAndInfoOfFilm()
{
    Film FilmOne = DemoMovie(L"Кошмар на улице вязов", 115, 1983, Genres::HORROR, 10);
    Film* structureFilmPointer = &FilmOne;
    wcout << *structureFilmPointer<< endl;
    structureFilmPointer->Name = L"Кошмар на улице вязов два";
    structureFilmPointer->YearOfIssue = 1986;
    structureFilmPointer->DurationInMinutes = 126;
    structureFilmPointer->Rating = 10.0;
    wcout << L" Новый " << *structureFilmPointer << endl;
    Film* structureFilmPointerTwo = &FilmOne;
    details::WriteAddress(structureFilmPointer, structureFilmPointerTwo);
}

void PrintAddressAndInfoOfTime()
{
    TimeStamp TimeOne = DemoTime(12, 53, 5);
    TimeStamp* structureTimePointer = &TimeOne;
    wcout << *structureTimePointer << endl;
    structureTimePointer->Hour = 13;
    structureTimePointer->Min = 54;
    structureTimePointer->Sec = 21;
    wcout << L" Новое " << *structureTimePointer << endl;
    TimeStamp* structureTimePointerTwo = &TimeOne;
    details::WriteAddress(structureTimePointer, structureTimePointerTwo);
}

void DemonstrationWrongPointers()
{
    Flight FlightOne = DemoFlight(L"Томск", L"Москва", 224);
    Film FilmOne = DemoMovie(L"Кошмар на улице вязов", 115, 1983, Genres::HORROR, 10);
    Film* structureFilmPointer = &FilmOne;
    Flight* structureFlightPointer = &FlightOne;
    // structureFilmPointer = &FlightOne;
    // structureFlightPointer = &FilmOne;
}

//2.2.5.
void Write(Rectangle& rectangle)
{
    wcout << rectangle << endl;
}


void Read(Rectangle& rectangle)
{
    rectangle.Length = GetValidValueFromUser<float>(L"Введите длину прямоугольника (положительное число):\n > ",
        [](auto const& val) {return val > 0; });
    rectangle.Width = GetValidValueFromUser<float>(L"Введите ширину прямоугольника (положительное число):\n > ",
        [](auto const& val) {return val > 0; });
}

void DemoReadAndWriteRectangles()
{
    array<Rectangle, 5> array;
    for (size_t i = 0; i < array.size(); i++)
    {
        wcout << L"Прямоугольник №" << (int)i + 1 << endl;
        Read(array[i]);
    }
    for (size_t i = 0; i < array.size(); i++)
    {
        wcout << L"Прямоугольник №" << (int)i + 1 << endl;
        Write(array[i]);
    }
}

void Exchange(Rectangle& rectangle1, Rectangle& rectangle2)
{
    Rectangle tmp{ rectangle2.Length, rectangle2.Width, rectangle2.Color };
    rectangle2 = DemoRectangle( rectangle1.Length, rectangle1.Width, rectangle1.Color);
    rectangle1 = DemoRectangle(tmp.Length, tmp.Width, tmp.Color);
}

void DemonstrationTask24254()
{
    Rectangle rectangleOne = DemoRectangle(2.5, 7, Colors::BROWN);
    Rectangle rectangleTwo = DemoRectangle(6, 20, Colors::PINK);
    Write(rectangleOne);
    Write(rectangleTwo);
    Exchange(rectangleOne, rectangleTwo);
    Write(rectangleOne);
    Write(rectangleTwo);
}


Rectangle FindRectangle(Rectangle* rectangles, size_t count, std::function<bool(Rectangle&, Rectangle&)> pred)
{
    Rectangle maxLengthRectangle = rectangles[0];
    for (size_t i = 1; i < count; i++)
    {
        if (pred(rectangles[i], maxLengthRectangle))
        {
            maxLengthRectangle = rectangles[i];
        }
    }
    return maxLengthRectangle;
}

void DemonstrationTask2255()
{
    array <Rectangle, 5> arrayOfRectangles;
    for (size_t i = 0; i < arrayOfRectangles.size(); i++)
    {
        Read(arrayOfRectangles[i]);
    }
    for (size_t i = 0; i < arrayOfRectangles.size(); i++)
    {
        wcout << L"Прямоугольник №" << (int)i + 1 << endl;
        Write(arrayOfRectangles[i]);
    }
    Rectangle* ptr = &arrayOfRectangles[0];
    Rectangle maxLengthRectangle = FindRectangle( ptr, arrayOfRectangles.size(),
        [](Rectangle& current, Rectangle& max){ return current.Length > max.Length; });
    wcout << L"C максимальной длинной "<< maxLengthRectangle << endl;
    Rectangle maxSquareRectangle = FindRectangle(ptr, arrayOfRectangles.size(),
     [](Rectangle& current, Rectangle& max){ return current.Length * current.Width > max.Length * max.Width; } );
    wcout << L"C максимальной площадью " << maxSquareRectangle << endl;
}

//2.2.6
void DemoDynamicFlight()
{
    auto flight = GetInfoOfFlightfromUser();
}

auto MakeFlight(wstring departurePoint, wstring destination, uint16_t flightTimeInMinutes)
{
    return unique_ptr<Flight>(new Flight{ departurePoint, destination, flightTimeInMinutes });
}

auto DemoDynamicFlights()
{
    vector<unique_ptr<Flight>> flights;
    for (size_t i = 0; i < 4; i++)
    {
        flights.push_back(GetInfoOfFlightfromUser());
    }
    for (auto& item : flights)
    {
        wcout << *item<<endl;
    }
    return flights;
}

void FindShortestFlight(vector<unique_ptr<Flight>>& flights)
{
    unique_ptr<Flight>& min = flights[0];
    for (auto& i : flights)
    {
        if (i->FlightTimeInMinutes < min->FlightTimeInMinutes)
        {
            min = MakeFlight(i->DeparturePoint, i->Destination, i->FlightTimeInMinutes);
        }
    }
    wcout << L" Самый быстрый" << *min << endl;
}

void DemonstrationTask226()
{
    vector<unique_ptr<Flight>> flights = DemoDynamicFlights();
    FindShortestFlight(flights);
}
//2.2.7

struct Circle // Структура круг
{
    double X; // X-координата центра круга
    double Y; // Y-координата центра круга
    double Radius; // Радиус
    string Color; // Цвет
};

Circle* MakeCircle(double x, double y, double radius, string color)
{
    return  new Circle{x, y, radius, color};
}

Circle* CopyCircle(Circle& circle)
{
    return new Circle{ circle.X, circle.Y, circle.Radius, circle.Color };
}

void DemoCircle()
{
    Circle* circle1 = MakeCircle(4.0, 7.0, 3.3, "red");
    Circle* circle2 = MakeCircle(2.0, -12.0, 12.75, "green");
    Circle* circle3 = MakeCircle(-10.0, 10.0, 1.45, "blue");
    Circle* copiedCircle1 = CopyCircle(*circle1);
    Circle* copiedCircle2 = CopyCircle(*circle2);
    Circle* copiedCircle3 = CopyCircle(*circle3);
}

auto MakeRectangle(float length, float width, Colors color)
{
    return unique_ptr<Rectangle>(new Rectangle{ length, width, color });
}

auto MakeFilm(wstring name,  uint16_t yearOfIssue, uint16_t duration, Genres genre, double rating)
{
    return unique_ptr<Film>(new Film{ name, yearOfIssue, duration, genre, rating });
}

auto MakeTime(uint16_t hour, uint16_t min, uint16_t sec)
{
    return unique_ptr<TimeStamp>(new TimeStamp{ hour, min, sec });
}

auto CopyRectangle(Rectangle& rectangleForCopy)
{
    return unique_ptr<Rectangle>(new Rectangle{ rectangleForCopy.Length, rectangleForCopy.Width, rectangleForCopy.Color });
}

auto CopyFlight(Flight& flightForCopy)
{
    return unique_ptr<Flight>(new Flight{ flightForCopy.DeparturePoint, flightForCopy.Destination,
        flightForCopy.FlightTimeInMinutes });
}

auto CopyFilm(Film& movieForCopy)
{
    return unique_ptr<Film>(new Film{ movieForCopy.Name, movieForCopy.YearOfIssue,
        movieForCopy.DurationInMinutes, movieForCopy.genre, movieForCopy.Rating });
}

auto CopyTime(TimeStamp& timeForCopy)
{
    return unique_ptr<TimeStamp>(new TimeStamp{ timeForCopy.Hour, timeForCopy.Min, timeForCopy.Sec });
}

//2.2.8 
enum class DayOfTheWeek
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

enum class StudentsForm
{
    FullTtime,
    PartTime,
    Evening,
    Distance
};

enum class SmartphoneManufacturers
{
    Apple,
    Samsung,
    Xiaomi,
    Nokia
};

enum class Season
{
    Fall,
    Winter,
    Spring,
    Summer
};


wstring WriteColor(Colors color)
{
    vector <wstring> nameColor{ L" желтый ", L" зеленый ", L" голубой ", L" коричневый",
        L" белый ", L" красный ", L" оранжевый ", L" розовый ", L" черный " };
    return nameColor[(int)color];
}

Colors ReadColor()
{
    wcout << L"Наберите номер необходимого вам цвета: " << endl
        << L" 0 - желтый,\n 1 - зеленый,\n 2 - голубой," << endl
        << L"3 - коричневый,\n 4 - белый,\n 5 - красный," << endl
        << L"6 - оранжевый,\n 7 - розовый,\n 8 - черный):" << endl;
    uint16_t userChoice = GetValidValueFromUser<uint16_t>(L" > ",
        [](auto& val) {return val >= 0 && val < 9; });
    return (Colors)userChoice;
}

template<size_t SIZE>
uint16_t CountColor(const array <Colors, SIZE>& colors, Colors  color)
{
    uint16_t count = 0;
    wcout << L"Массив цветов:";
    for (auto item : colors)
    {
        wcout << L" " << WriteColor(item)<<L",";
        if (item == color)
        {
            count++;
        }
    }
    wcout << endl;
    return count;
}

void DemoEnums()
{
    Colors colorOne = Colors::RED;
    DayOfTheWeek day = DayOfTheWeek::Saturday;
    Genres movieGenre = Genres::HORROR;
    StudentsForm studentOne = StudentsForm::FullTtime;
    SmartphoneManufacturers smartfoneOne = SmartphoneManufacturers::Apple;
    Season season = Season::Summer;
    array <Colors,6> colors{ Colors::BLACK, Colors::PINK, Colors::RED,
                     Colors::RED, Colors::WHITE, Colors::RED };
    array <DayOfTheWeek,7> daysGone{ DayOfTheWeek::Monday, DayOfTheWeek::Saturday, DayOfTheWeek::Friday,
                             DayOfTheWeek::Sunday, DayOfTheWeek::Thursday, DayOfTheWeek::Tuesday,
                             DayOfTheWeek::Wednesday };
    array <Genres,4> lovelyGenres{ Genres::HORROR, Genres::DOCUMENTARY, Genres::DRAMA, Genres::THRILLER };
    array <StudentsForm,3> forms{ StudentsForm::Distance, StudentsForm::Evening, StudentsForm::PartTime };
    array <SmartphoneManufacturers,5> smartfones{ SmartphoneManufacturers::Apple, SmartphoneManufacturers::Apple,
                                          SmartphoneManufacturers::Samsung, SmartphoneManufacturers::Xiaomi,
                                          SmartphoneManufacturers::Nokia };
    array <Season, 3> seasonGone{ Season::Winter, Season::Spring, Season::Summer };
    wcout  << L"Количество элементов массива красного цвета:" << CountColor(colors, Colors::RED);
}

//2.2.9.

auto DemoCustomMovies()
{
    vector<unique_ptr<Film>> films;
    for (size_t i = 0; i < 5; i++)
    {
        films.push_back(GetInfoOfFilmFromUser());
    }
    for (auto& item : films)
    {
        wcout << *item<<endl;
    }
    return films;
}

auto DemoDynamicMovies()
{
    vector<unique_ptr<Film>> films;
	//TODO: год перепутан с продолжительностью
    films.push_back(MakeFilm(L"Мстители", 210, 2010, Genres::ADVENTURE, 7.9));
    films.push_back(MakeFilm(L"Реинкарнация", 145, 2018, Genres::HORROR, 7.5));
    films.push_back(MakeFilm(L"Джиа", 210, 2003, Genres::DRAMA, 8.2));
    films.push_back(MakeFilm(L"Кошмар на улице вязов", 115, 1983, Genres::HORROR, 10));
    films.push_back(MakeFilm(L"Крестный отец", 188, 1972, Genres::DRAMA, 7.9));
    films.push_back(MakeFilm(L"Бойцовский клуб", 210, 1999, Genres::THRILLER, 10));
    films.push_back(MakeFilm(L"На игле", 207, 1995, Genres::DRAMA, 7.9));
    films.push_back(MakeFilm(L"Криминальное чтиво", 210, 1994, Genres::COMEDY, 8.9));
    films.push_back(MakeFilm(L"Бегущий по лезвию", 210, 1982, Genres::THRILLER, 10));
    films.push_back(MakeFilm(L"Чужие", 210, 1986, Genres::HORROR, 8.0));
    for (auto& item : films)
    {
        wcout << *item<<endl;
    }
    return films;
}

uint16_t CountMoviesByGenre(vector<unique_ptr<Film>>& films, Genres findGenre)
{
    uint16_t count = 0;
    for (auto& item : films)
    {
        if (item->genre == findGenre)
            count++;
    }
   return count;
}
auto FindBestGenreMovie(vector<unique_ptr<Film>>& films, Genres findGenre)
{
    auto movie = make_unique<Film>();
    movie = nullptr;
    double maxRating = 0;
    for (auto& item : films) 
    {
        if (item->genre == findGenre)
        {
            if (item->Rating >= maxRating)
            {
                movie = MakeFilm(item->Name, item->DurationInMinutes, item->YearOfIssue,
                    item->genre, item->Rating);
                maxRating = movie->Rating;
            }
        }
    }
       
    if (movie != nullptr)
    {
        wcout << L" Лучший фильм в жанре" << GenreNameForConsole(movie->genre)
            << L":" << *movie << endl;
    }
    else
    {
        wcout << L"Фильмов такого жанра нет в коллекции.";
    }
    return movie;
}

void DemonstrationTask229()
{
    auto movie = DemoDynamicMovies();
    wcout << L"Количество фильмов в жанре ужасы: " << CountMoviesByGenre(movie, Genres::HORROR) << endl;
    FindBestGenreMovie(movie, Genres::HORROR);
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
	
    return EXIT_SUCCESS;
}

//TODO: ознакомься с понятием gitignore. Потому что в репозиторий попадает всякий трэш
