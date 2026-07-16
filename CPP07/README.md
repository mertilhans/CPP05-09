# ex01 — iter

## Tasarım

`iter` tek bir function template olarak yazıldı:

```cpp
template <typename t, typename func>
void iter(t *array, const unsigned int len, func f)
```

### Neden 2 tane typename var (`t` ve `func`)

İki template parametresine ihtiyaç var çünkü fonksiyonun iki farklı yerde,
birbirinden bağımsız iki tip serbest bırakılıyor:

- `t` → dizinin eleman tipi. `iter` "herhangi bir tipte dizi" ile çalışmalı
  (subject: "must work with any type of array"), bu yüzden sabit bir tip
  (örn. sadece `int`) yazılamaz, template parametresi olmak zorunda.
- `func` → 3. parametre olan fonksiyonun tipi. Bu da sabit tek bir fonksiyon
  imzasına bağlanamaz, çünkü hem farklı elemana uygulanan farklı fonksiyonları
  (örn. yazdıran, değiştiren, `const &` alan, `&` alan) hem de fonksiyon
  pointer'ı ya da fonksiyon objesi gibi farklı çağrılabilir şeyleri kabul
  etmesi gerekiyor (subject: "can be an instantiated function template").

Uzunluk parametresi (`len`) için ayrı bir typename yok, çünkü onun tipi sabit
tutuluyor (`const unsigned int`) — dizinin uzunluğu her zaman aynı kategoriden
bir sayı, tipini değiştirmeye gerek yok. Yani template parametresi sayısı,
"gerçekten değişken olması gereken tip sayısı" kadar: dizi elemanı ve fonksiyon.

### `t` template olmasaydı ne olurdu

`t`'yi kaldırıp sabit bir tip yazsaydık (örn. `void iter(int *array, ...)`),
`iter` sadece `int` dizileriyle çalışırdı. Bunu gerçekten deneyip derledim —
`std::string` dizisiyle çağırınca:

```cpp
void iter(int *array, const unsigned int len, func f)
...
std::string arr[3] = {"a", "b", "c"};
iter(arr, 3, printStr);
```
```
error: no matching function for call to 'iter'
note: candidate function template not viable: no known conversion from
'std::string[3]' to 'int *' for 1st argument
```

Yani `t` olmasaydı "herhangi bir tipte dizi" şartı (subject'in istediği) daha
en baştan imkansız olurdu — her farklı eleman tipi için ayrı bir `iter`
fonksiyonu yazman gerekirdi.

### `func` template olmasaydı ne olurdu

`func`'ı kaldırıp imzayı sabit bir fonksiyon pointer tipine bağlasaydık
(örn. `void iter(t *array, const unsigned int len, void (*f)(t &))`), bu
sefer sadece **tam o imzaya uyan düz fonksiyonlar** kabul edilirdi — bir
"functor" (yani `operator()` tanımlı bir sınıf nesnesi) veremezdin. Bunu da
denedim:

```cpp
class Printer {
public:
    void operator()(int &x) { std::cout << x << " "; }
};
...
Printer p;
iter(arr, 3, p);   // func sabit "void (*)(t &)" olsaydı:
```
```
error: no matching function for call to 'iter'
note: candidate template ignored: could not match 'void (*)(t &)' against 'Printer'
```

Aynı çağrıyı gerçek `iter.hpp` (yani `func` template olarak) ile yaptığımda
sorunsuz derlendi ve çalıştı (`1 2 3` yazdırdı). Yani `func` template
olmasaydı, subject'in "3. parametre bir fonksiyon template'i olabilir"
(instantiated function template kabul etme) şartı karşılanamazdı — sadece
tek bir sabit imzaya uyan çağrılabilir şeyler kullanılabilirdi.

Ayrı bir const/non-const overload'a gerek kalmadı. Sebep: `t` tipi çağrı sırasında
diziden deduce ediliyor. `const int*` bir dizi geçilirse `t` otomatik olarak
`const int` olur, `array[i]` de `const int` üretir. Böylece:

- Normal dizi + elemanı değiştiren fonksiyon (`int &`) → çalışır.
- `const` dizi + sadece okuyan fonksiyon (`const int &`) → çalışır.
- `const` dizi + elemanı değiştirmeye çalışan fonksiyon (`int &`) → **derleme hatası**
  (`binding reference of type 'int' to value of type 'const int' drops 'const'
  qualifier`), yani yanlış kullanım derleyici tarafından engelleniyor.

### Neden `<int>`, `<float>` gibi açık tip belirtiyoruz

`print<int>`, `increment<int>`, `Array<int>` gibi köşeli parantez içinde tip
yazmanın sebebi, **derleyicinin tipi kendi başına çıkaramadığı** durumlar için
onu açıkça belirtmek. İki farklı senaryo var:

**1. Fonksiyon template'ini bir değer olarak geçerken (`print<int>` gibi)**

`print<int>` yazmadan, sadece `print` yazıp `iter`'a geçmeye çalışırsam:

```cpp
::iter(arr, 4, print);   // <int> yok
```
```
error: no matching function for call to 'iter'
note: candidate template ignored: couldn't infer template argument 'func'
```

Neden olmuyor: `print` çağrılmıyor burada, sadece bir **değer olarak başka bir
fonksiyona parametre** olarak veriliyor. Normalde template argument deduction,
bir fonksiyonu **çağırdığında** verdiğin argümanların tipine bakarak çalışır
(`print(x)` dersen `x`'in tipinden `t`'yi çıkarır). Ama burada `print`'i
çağırmıyoruz, sadece `iter`'a bir "fonksiyon değeri" olarak yolluyoruz — ve
`iter`'ın `func` parametresi zaten kendisi de generic (herhangi bir tip
olabilir), yani deduction için elinde hiçbir ipucu yok. Bu yüzden hangi
`print` versiyonunu (int için mi, string için mi, ...) kastettiğimi
`<int>` ile açıkça söylemem gerekiyor.

**2. Bir class template'ten nesne oluştururken (`Array<int>` gibi)**

```cpp
Array numbers(10);   // <int> yok
```
```
error: use of class template 'Array' requires template arguments
```

Bu tamamen farklı bir sebepten: **C++98'de class template'ler için argument
deduction diye bir şey yok** (bu özellik ancak C++17 ile geldi — "Class
Template Argument Deduction"). Yani bir fonksiyon template'inde bazen
deduction çalışsa da, bir class template'ten nesne oluştururken (`Array`
gibi) tipi **her zaman elle yazman şart** — derleyicinin constructor'a
verdiğin `10` gibi bir argümandan `T`'nin ne olduğunu çıkarma yetkisi/izni
C++98 standardında yok.

Özetle: fonksiyon template'lerinde deduction bazı durumlarda (özellikle
"değer olarak geçirme" senaryosunda) yetersiz kalabilir; class template'lerde
ise (C++98'de) deduction'ın kendisi hiç yok — ikisinde de çözüm aynı: tipi
`<...>` ile açıkça yazmak.

## Test sonuçları

`-Wall -Wextra -Werror -std=c++98` ile:

- `iter(arr, 5, printInt)` (non-const dizi, non-const ref fonksiyon) → derlendi, doğru çalıştı.
- `iter(carr, 5, printConstInt)` (const dizi, const ref fonksiyon) → derlendi, doğru çalıştı.
- `iter(carr, 5, printInt)` (const dizi, non-const ref fonksiyon) → beklendiği gibi
  derleme hatası verdi.

`size_t i = -1; while (++i < len)` deyimi, unsigned taşma (well-defined modular
arithmetic) ile 0'dan başlıyor, uyarı üretmiyor.

# ex02 — Array

## Class template nedir

Şimdiye kadar (`swap`, `min`, `max`, `iter`) hep **fonksiyon template**
kullandık — yani template olan tek şey, tek bir fonksiyondu. **Class
template** ise bunun sınıflar için karşılığı: bir class'ın tamamını (bütün
üye değişkenlerini ve üye fonksiyonlarını) bir tipe göre genelleştirmek.

```cpp
template <typename T>
class Box
{
    T _value;
public:
    Box(T value) : _value(value) {}
    T get() const { return _value; }
};
```

Burada `T`, class'ın **her yerinde** kullanılabilir — üye değişkenlerin tipi
olabilir, üye fonksiyonların parametre/dönüş tipi olabilir. Fonksiyon
template'inde `T` sadece o **tek fonksiyonun** kapsamındaydı (örn. `iter`'ın
`t`'si sadece `iter`'ın içinde geçerliydi); class template'te ise `T`, class
`{ }` içindeki **her şeyin** ortak, paylaşılan tipi.

Kullanırken de fark var: bir fonksiyon template'i çağırırken derleyici genelde
tipi argümanlardan **deduce edebiliyordu** (`::min(a, b)` dediğinde `a`'nın
tipinden `t`'yi anlıyordu). Class template'te ise (C++98'de) nesne
oluştururken tipi **her zaman elle** yazman gerekiyor — `Box<int> b(5);` gibi
(bunun sebebini yukarıda "Neden `<int>` yazıyoruz" bölümünde zaten
detaylandırmıştık: C++98'de class template'ler için argument deduction yok).

`Array<T>` de tam olarak bu mantıkla çalışıyor: `T` hem `_data` (elemanları
tutan pointer) hem `operator[]`'in döndürdüğü referans hem de diğer bütün
üye fonksiyonların imzasında ortak, tek bir tip olarak geçiyor. `Array<int>`
dediğinde bütün class, sanki içindeki her `T` yerine `int` yazılmış gibi
davranıyor; `Array<std::string>` dediğinde de aynı class, bu sefer `T` yerine
`std::string` geçmiş gibi davranıyor — kodu tekrar yazmana gerek kalmadan.

## `delete` yerine `delete[]` yazmazsan ne olur

Destructor'da `_data`'yı serbest bırakırken **`delete[] _data;`** yazmak
zorundayız, sadece `delete _data;` (köşeli parantezsiz) yazmak yeterli değil
— çünkü `_data`, `new t[n]` ile (bir **dizi** olarak) ayrılmıştı. Bunu somut
görmek için bir class (her elemanında destructor çağrıldığında ekrana yazan)
ile test ettim:

```cpp
class Loud {
public:
    int id;
    ~Loud() { std::cout << "destructor calisti (id=" << id << ")" << std::endl; }
};

Loud *arr1 = new Loud[3];
delete[] arr1;   // dogru

Loud *arr2 = new Loud[3];
delete arr2;     // yanlis, [] yok
```

Derleyici bile bunu uyarıyor:
```
warning: 'delete' applied to a pointer that was allocated with 'new[]';
did you mean 'delete[]'?
```

Ve çalıştırınca fark çok net:
```
=== delete[] ile (dogru) ===
destructor calisti (id=3)
destructor calisti (id=2)
destructor calisti (id=1)
=== delete ile (yanlis, [] olmadan) ===
destructor calisti (id=1)
```
(bu satırdan sonra program **çöktü** — exit code 133)

Yani `delete[]` her 3 elemanın destructor'ını da çağırıyor (doğru davranış).
`delete` (parantezsiz) ise **sadece 1 elemanın** destructor'ını çağırıyor,
diğer ikisi hiç temizlenmiyor, ve hemen ardından program çöküyor. Sebep:
`new[]` ile ayrılan bellek, başında (görünmez şekilde) "kaç eleman var"
bilgisini de tutuyor; `delete` (tekil sürüm) bu bilgiyi doğru yorumlamıyor,
hem eksik destructor çağrısı yapıyor hem de bellek yöneticisinin (heap
allocator) iç muhasebesini bozup çökmeye yol açıyor. Bu yüzden `Array`'in
destructor'ında **mutlaka `delete[]`** kullanmamız gerekiyor, `new t[n]`
ile simetrik olacak şekilde.

## `Array.hpp` sonuna `#include "Array.tpp"` eklemezsen ne olur

İmplementasyonları (`Array.hpp`'deki deklarasyonlara karşılık gelen
fonksiyon gövdelerini) ayrı bir `Array.tpp` dosyasına yazdık. Ama `Array.hpp`
bu dosyayı **kendi sonunda include etmediği sürece**, `main.cpp` sadece
`Array.hpp`'yi include ettiğinde derleyici fonksiyonların **sadece isimlerini**
görüyor, **gövdelerini** hiç görmüyor. Bunu gerçekten deneyip linker
hatasını aldım:

```
Undefined symbols for architecture arm64:
  "Array<int>::Array(Array<int> const&)", referenced from: ...
  "Array<int>::Array(unsigned int)", referenced from: ...
  "Array<int>::~Array()", referenced from: ...
  "Array<int>::operator[](int)", referenced from: ...
ld: symbol(s) not found for architecture arm64
```

Bu **derleme (compile) hatası değil, linker hatası** — kod sözdizimi olarak
tamamen doğru, `main.cpp` derlenip `.o` dosyası üretiliyor, ama linker
"`Array<int>::operator[]` diye bir fonksiyon çağrılıyor ama tanımı hiçbir
yerde yok" diyor. Sebep: template fonksiyonların implementasyonu, normal
fonksiyonlardan farklı olarak, **kullanıldığı her yerde** (her `.cpp`
dosyasında ayrı ayrı) derleyici tarafından görülüp yeniden üretilmesi
gerekiyor (instantiation) — bu da ancak gövdenin de o `.cpp`'ye (include
zinciri üzerinden) ulaşmasıyla mümkün.

**Çözüm:** `Array.hpp`'nin en sonuna (`#endif`'ten hemen önce) şunu eklemek:
```cpp
#include "Array.tpp"
```
Böylece `Array.hpp`'yi include eden her dosya, otomatik olarak `Array.tpp`'yi
de (implementasyonları) include etmiş oluyor — `.hpp` "arayüz", `.tpp`
"gövde" ama derleyici ikisini birden, tek bir dosyaymış gibi görüyor.

## Durum: `Array` implementasyonu tamamlandı, test edildi

`#include "Array.tpp"` eklendikten sonra tüm `Array<T>` implementasyonunu
(constructor'lar, `operator=`, `operator[]`, destructor, `size()`) mevcut
`main.cpp` (deep-copy testi + `numbers[-2]`/`numbers[MAX_VAL]` sınır dışı
testi) ile uçtan uca test ettim:

- `-Wall -Wextra -Werror -std=c++98` ile derlendi (bir workaround ile,
  aşağıya bak), çalıştı, **exit code 0**.
- Çıktı: iki kere `std::exception` (iki `catch` bloğu da doğru tetiklendi).
- "didn't save the same value!!" hatası hiç tetiklenmedi → deep copy doğru
  çalışıyor, `tmp`/`test` kopyalarının scope sonunda destroy olması orijinal
  `numbers`'ı bozmadı.
- **AddressSanitizer** (`-fsanitize=address`) ile de çalıştırdım, hiçbir
  buffer overflow / use-after-free / double-free bulunamadı (macOS'ta
  leak-detection desteklenmiyor, ama bellek bozulması kontrolleri temiz
  çıktı).

**Bulunan ama henüz düzeltilmemiş bir sorun — `main.cpp:2`:**
```cpp
#include <Array.hpp>
```
Köşeli parantez (`<...>`) kullanılmış, kendi (yerel) header'lar için
**tırnak** (`"Array.hpp"`) kullanılması gerekiyordu. Gerçek `make re` ile
denedim, tam olarak bu yüzden derleme başarısız oluyor:
```
main.cpp:2:10: error: 'Array.hpp' file not found with <angled> include;
use "quotes" instead
```
(Kendi testlerimde `-I.` ekleyerek bu sorunu atlattım, ama asıl teslim
edilecek `Makefile`'da böyle bir flag yok — yani `main.cpp:2`'yi
`#include "Array.hpp"` olarak değiştirmek lazım, yoksa `make` çalışmaz.)
