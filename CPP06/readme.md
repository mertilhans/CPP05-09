# CPP06 — C++ Cast'leri

Bu modül C++'ın dört tip dönüşüm (cast) operatörünü işliyor: `static_cast`, `dynamic_cast`,
`reinterpret_cast`, `const_cast`. C tarzı `(int)x` cast'i hepsini tek çatı altında topladığı
için hem tehlikeli hem de niyeti belli etmiyor; C++ bunları ayrı operatörlere bölerek hem
derleyicinin kontrol etmesini hem de okuyanın "burada ne oluyor" sorusuna cevap bulmasını
sağlıyor.

---

## Önce temel: bir C++ programı RAM'i nasıl bölüyor?

Cast'lerin "nerede ne tutuluyor" sorusuna cevap verebilmek için önce bir process'in bellek
(RAM) düzenini bilmek gerekiyor. Program çalışırken kendisine ayrılan sanal adres alanı
kabaca şöyle bölünüyor (adresler düşükten yükseğe):

```
düşük adres
┌───────────────────────────┐
│  Text / Code segment      │  derlenmiş makine kodu (fonksiyonlar) + vtable'lar,
│  (read-only)              │  RTTI type_info nesneleri, string literalleri
├───────────────────────────┤
│  Data segment              │  ilklendirilmiş global/static değişkenler
├───────────────────────────┤
│  BSS segment                │  ilklendirilmemiş global/static değişkenler
├───────────────────────────┤
│  Heap                       │  new / malloc ile ayrılan; sen delete/free
│  (aşağıdan yukarıya büyür)  │  edene kadar yaşar
│            ...              │
│  (boş alan)                 │
│            ...              │
│  Stack                      │  fonksiyon çağrıları, local değişkenler;
│  (yukarıdan aşağıya büyür)  │  fonksiyon dönünce otomatik silinir
└───────────────────────────┘
yüksek adres
```

Bu proje boyunca hep aynı üç bölgeye referans vereceğiz: **stack** (local değişkenler),
**heap** (`new` ile ayrılanlar) ve **text/read-only segment** (kod + vtable + RTTI).

---

## Cast tipleri

### `static_cast`
- **Ne zaman:** Derleme zamanında (compile-time) tipin doğruluğu biliniyorsa. Sayısal
  tipler arası dönüşüm (`int` ↔ `float` ↔ `double` ↔ `char`), örtük dönüşüme izin veren
  sınıflar arası dönüşüm (örn. taban sınıfa yukarı cast — *upcast*), `void*`'i geri
  orijinal tipine çevirmek.
- **Runtime kontrolü yok.** Yanlış kullanırsan (örn. alakasız iki sınıf arası) derleyici
  hata verir ama tip uyuşmazlığını çalışma zamanında yakalamaz. Hiçbir ek bellek/RAM
  kaydı gerektirmez — RTTI'ye ihtiyaç duymaz.
- **Bellekte ne olur:** Sayısal cast'lerde bu bir **no-op değildir** — CPU gerçekten bit
  kalıbını değiştiren bir işlem (instruction) üretir. Örn. `int`→`float` cast'inde CPU'nun
  FPU birimi, ikili tabanda tutulan tamsayıyı IEEE-754 float formatına **yeniden kodlar**;
  `reinterpret_cast`'in aksine "aynı bitleri farklı yorumlamak" değil, gerçek bir
  dönüşümdür.
- **Basit örnek:** 5 elmayı 2 kişiye bölüştürüyorsun.
  ```cpp
  int elma = 5, kisi = 2;
  std::cout << elma / kisi << std::endl;                          // 2  (küsürat gitti)
  std::cout << static_cast<float>(elma) / kisi << std::endl;      // 2.5
  ```
  `int / int` işlemi küsüratı atıyor; `static_cast<float>` sayıyı önce ondalıklıya
  çevirdiği için bölme de ondalıklı sonuç veriyor. Günlük hayattaki karşılığı: tam
  sayı gözlüğünü çıkarıp ondalıklı gözlüğü takmak.
- **Bu projede:** `ex00/ScalarConverter.cpp` içinde `char`→`int`→`float`→`double`
  dönüşümlerinin tamamı `static_cast` ile yapılıyor (örn. `static_cast<float>(value)`).

```cpp
double d = 42.9;
int i = static_cast<int>(d); // i == 42, kesin ve öngörülebilir
```

### `dynamic_cast`
- **Ne zaman:** Polimorfik bir sınıf hiyerarşisinde (en az bir `virtual` fonksiyon olmalı)
  **çalışma zamanında** bir nesnenin gerçek tipini öğrenmek/doğrulamak istediğinde. Tipik
  kullanım: taban sınıf pointer/referansını türetilmiş sınıfa aşağı cast etmek (*downcast*).
- **Runtime kontrolü var.** Bunun bedelini RTTI (Run-Time Type Information) ile öder:
  - Pointer üzerinde başarısız olursa **`NULL` döner**.
  - Referans üzerinde başarısız olursa **`std::bad_cast` exception fırlatır**.
- **RAM'de nerede tutuluyor (mekanizmanın detayı):** `Base` sınıfı `virtual ~Base()`
  içerdiği için polimorfik oluyor ve derleyici her `A`/`B`/`C` nesnesine görünmez, gizli
  bir üye ekliyor: **vptr** (virtual table pointer). Bu vptr nesnenin bellekteki ilk
  alanıdır (nesne stack'te ya da heap'te olsa da fark etmez, vptr onun içindedir):

  ```
  new A() ile heap'te ayrılan bir A nesnesi:

      heap adresi X  ┌─────────────────────┐
                      │ vptr  (8 byte)      │──┐   nesnenin kendi verisi yok (A boş),
                      └─────────────────────┘  │   sadece bu gizli pointer var
                                                │
      text/read-only segment (kod ile birlikte)│
                      ┌─────────────────────┐  │
                      │ A'nın vtable'ı       │◄─┘
                      │  - type_info(A)*     │  RTTI burada saklanıyor
                      │  - ~A() adresi       │
                      └─────────────────────┘
  ```

  `dynamic_cast<A*>(obj)` çalıştığında CPU şunu yapar: `obj`'nin vptr'sinden vtable'a,
  vtable'dan o sınıfın `type_info` nesnesine gider ve hedef tip (`A`) ile karşılaştırır;
  eşleşmezse hiyerarşiyi yukarı doğru (taban sınıflara) dolaşmaya devam eder. Bu yüzden
  `dynamic_cast`, `static_cast`'ten daha yavaştır — gerçek bir bellek okuma/karşılaştırma
  zinciri çalıştırır. `type_info` nesneleri ve vtable'lar **stack'te ya da heap'te değil**,
  programın **read-only/text segmentinde** (kodla birlikte, derleme anında üretilmiş
  sabit veri olarak) yaşarlar — bu yüzden her `A` nesnesi kendi vtable kopyasını taşımaz,
  hepsi aynı tek vtable'ı (aynı RAM adresini) paylaşır.

- **Basit örnek:** elinde üstünde sadece "Hayvan" yazan bir kutu var, içinde gerçekte
  köpek mi kedi mi olduğunu bilmiyorsun.
  ```cpp
  class Animal { public: virtual ~Animal() {} };
  class Dog : public Animal {};
  class Cat : public Animal {};

  Animal *pet = new Dog();

  if (dynamic_cast<Dog*>(pet))
      std::cout << "Bu bir kopek" << std::endl;
  else
      std::cout << "Kopek degil" << std::endl;
  // ciktisi: "Bu bir kopek"
  ```
  `dynamic_cast` kutuyu açıp "gerçekten köpek mi?" diye kontrol etmenin güvenli
  yoludur; yanlış tahmin edersen (örn. `Cat*`'e çevirmeye çalışırsan) programı
  çökertmez, sadece `NULL` (ya da referansta exception) döner.
- **Bu projede:** `ex02/main.cpp` içindeki `identify()` fonksiyonlarının tamamı bunun
  üzerine kurulu:

```cpp
// pointer versiyonu -> NULL kontrolü
if (dynamic_cast<A*>(obj)) std::cout << "A" << std::endl;

// referans versiyonu -> try/catch zorunlu
try {
    (void)dynamic_cast<A&>(obj);
    std::cout << "A" << std::endl;
} catch (...) { /* bu tip değilmiş, sıradakini dene */ }
```
  `Base` sınıfının `virtual ~Base()` yıkıcısı olmasaydı bu hiç derlenmezdi —
  `dynamic_cast` polimorfik olmayan (virtual fonksiyonu olmayan) bir tip üzerinde
  kullanılamaz, çünkü o zaman vptr/vtable diye bir şey var olmaz.

### `reinterpret_cast`
- **Ne zaman:** Birbiriyle hiçbir ilişkisi olmayan tipler arasında **ham bit kalıbını
  olduğu gibi** yeniden yorumlamak istediğinde. En klasik örnek: bir pointer'ı bir tamsayıya
  (`uintptr_t`) veya başka bir pointer tipine çevirmek.
- **En tehlikeli olanı budur** — hiçbir tip/güvenlik kontrolü yapmaz, sadece derleyiciye
  "bu bitleri şu tipmiş gibi davran" der. Yanlış kullanımı tanımsız davranışa (UB) yol açar.
- **Bellekte ne olur:** `static_cast`'in aksine bu gerçekten **no-op**'tur — CPU hiçbir
  dönüştürme işlemi yapmaz, RAM'deki bitler bir bayt bile değişmez. Sadece derleyicinin
  "bu adresi/değeri şu tip olarak oku" diye tuttuğu tip etiketi değişir. Pointer↔tamsayı
  cast'inde adresin kendisi (RAM'deki sayısal konum) aynen korunur.
- **Basit örnek:**
  ```cpp
  int sayi = 65;
  char *harfGibi = reinterpret_cast<char*>(&sayi);
  std::cout << *harfGibi << std::endl;   // 'A' basar (65, ASCII'de 'A' demek)
  ```
  Bellekteki hiçbir şey değişmedi — aynı 4 byte'a bu sefer "bir tam sayı" değil "bir
  harf" gözüyle baktık. Aynı fotoğrafa farklı bir çerçeve takmak gibi düşün: fotoğraf
  (bitler) aynı, sadece çerçeve (tip) değişti.
- **Bu projede:** `ex01/Serializer.cpp` bunu tam da amaçlandığı gibi kullanıyor: bir
  `Data*`'yı `uintptr_t`'ye (serileştirme), sonra geri `Data*`'ye (deserileştirme) çeviriyor.
  Adres bit kalıbı hiç değişmediği için `restored == original` garanti.

```cpp
uintptr_t raw = reinterpret_cast<uintptr_t>(ptr); // pointer -> tamsayı
Data* back    = reinterpret_cast<Data*>(raw);     // tamsayı -> pointer
```
  `uintptr_t`, pointer'ı bilgi kaybı olmadan tutabilecek büyüklükte bir tamsayı tipi
  (`<cstdint>`); bu yüzden `int` değil `uintptr_t` kullanılıyor (64-bit sistemde pointer
  8 byte, `int` sadece 4 byte — sığmazdı).

### `const_cast`
- **Ne zaman:** Bir değişkenin `const`/`volatile` niteleyicisini eklemek ya da **kaldırmak**
  için. Tipi değiştirmez, sadece const'luğu değiştirir.
- **Bellekte ne olur:** Hiçbir şey — bu tamamen derleyici seviyesinde (compile-time) bir
  etikettir, RAM'de const/non-const diye ayrı bir bit yoktur. Tehlike şurada: eğer
  nesne **gerçekten** `const` olarak (örn. read-only/data segmentte, ya da derleyicinin
  onu register'da tuttuğu bir yerde) tanımlandıysa, `const_cast` ile o değeri değiştirmeye
  çalışmak tanımsız davranıştır.
- **Basit örnek:**
  ```cpp
  void yazdir(char *s) { std::cout << s << std::endl; } // eski fonksiyon, const almıyor
                                                          // ama içeride s'i değiştirmiyor da

  const char *mesaj = "merhaba";
  yazdir(const_cast<char*>(mesaj));   // "merhaba" yazdirir, mesaj bozulmaz
  ```
  "Bu kutuya dokunma" etiketini geçici olarak çıkarıp eski bir alete uydurmak gibi —
  ama etiketi çıkardın diye içeriği gerçekten değiştirirsen (mesela `mesaj`'ın kendisi
  gerçekten sabit bir yerde tutuluyorsa) bu güvenli olmaktan çıkar.
- **Bu modülde kullanılmıyor** ama tamamlık için: genelde eski/const-incorrect bir API'ye
  const olmayan bir referans geçmek zorunda kalındığında kullanılır.

### Özet tablo

| Cast              | Kontrol zamanı | Bellekte üretilen işlem              | Ne için                          | Başarısızlıkta       |
|-------------------|----------------|----------------------------------------|-----------------------------------|-----------------------|
| `static_cast`     | derleme        | gerçek dönüşüm (bit kalıbı değişir)   | ilgili/sayısal tipler arası       | derleme hatası        |
| `dynamic_cast`    | çalışma zamanı | vtable/RTTI üzerinden zincir karşılaştırma | polimorfik downcast          | `NULL` / `bad_cast`   |
| `reinterpret_cast`| yok            | no-op (bit tek bir bayt bile değişmez)| alakasız tipler, ham bit çevirimi | tanımsız davranış     |
| `const_cast`      | yok            | no-op (sadece derleyici etiketi)      | const/volatile ekle-kaldır        | UB (gerçek const'u değiştirirsen) |

---

## ex00 — ScalarConverter

`char`, `int`, `float`, `double` arasında bir string literalinden yola çıkarak dönüşüm
yapan, örneklenemeyen (instantiate edilemez) bir yardımcı sınıf. Constructor `private` ve
tanımsız bırakılıyor, tek üye fonksiyon `static void convert(const std::string&)`.

### Literal tipi nasıl tespit ediliyor

| Girdi örneği        | Tespit kuralı                                                        |
|---------------------|-----------------------------------------------------------------------|
| `'a'`, `a`          | uzunluk == 1 ve rakam değil → `char` (tırnak istemiyor, direkt `a`)   |
| `42`, `+42`, `-42`   | tamamen rakam, başında opsiyonel `+`/`-` → `int`                      |
| `4.2f`, `-3.14f`     | sonu `f`, geri kalanı sayısal + en fazla bir `.` → `float`            |
| `4.2`, `-3.14`       | içinde `.` var, tamamen sayısal → `double`                           |
| `nanf`, `+inff`, `-inff` | özel **pseudo-literal float** değerleri                          |
| `nan`, `+inf`, `-inf`    | özel **pseudo-literal double** değerleri                         |
| yukarıdakilerin hiçbiri | `impossible` — hepsi için                                        |

### Bu tipler RAM'de gerçekte kaç byte ve nasıl duruyor?

`convert()` içindeki `i`, `f`, `d` gibi local değişkenler fonksiyon çalıştığı sürece
**stack**'te yaşar (fonksiyondan çıkınca otomatik silinirler, `delete` gerekmez).

| Tip      | Boyut (tipik x86-64) | Bellekteki temsili                                              |
|----------|-----------------------|-------------------------------------------------------------------|
| `char`   | 1 byte                | doğrudan ASCII kodu (örn. `'*'` → `0x2A`)                         |
| `int`    | 4 byte                | ikiye tümleyen (two's complement) tam sayı                        |
| `float`  | 4 byte                | IEEE-754 tek hassasiyet: 1 işaret + 8 üs (exponent) + 23 mantis bit |
| `double` | 8 byte                | IEEE-754 çift hassasiyet: 1 işaret + 11 üs (exponent) + 52 mantis bit |

IEEE-754 bit düzeni (float örneği):

```
 bit:  31        30-23           22-0
      ┌─┬─────────────────┬───────────────────────┐
      │S│     exponent    │        mantissa       │
      └─┴─────────────────┴───────────────────────┘
       işaret   üs (bias'lı)      kesir kısmı
```

### `nan` / `inf` pseudo-literal'ları nedir ve bitlerde nasıl görünür?

Bunlar sayı **değil**, yukarıdaki IEEE-754 bit düzeninin özel (reserved) durumlarıdır.

**Basit örnek:** okulda 0'a bölme yasaktı, "tanımsız" derdik. Bilgisayar da aynı şeyi
yapıyor, sadece hata verip durmak yerine bu duruma özel bir isim veriyor:
```cpp
double sonsuz    = 1.0 / 0.0; // -> inf   ("sonsuz büyük" gibi düşün)
double tanimsiz  = 0.0 / 0.0; // -> nan   ("bu bir sayı değil" gibi düşün)
```

- **`inf` (sonsuzluk):** exponent alanının **tamamı 1** ve mantissa'nın **tamamı 0**
  olduğunda ortaya çıkar. İşaret biti 0 ise `+inf`, 1 ise `-inf`. Örn. `1.0/0.0` bu
  bit kalıbını üretir.
- **`nan` (Not a Number):** exponent alanı yine **tamamı 1**, ama mantissa **sıfırdan
  farklı** olduğunda ortaya çıkar (örn. `0.0/0.0`, `sqrt(-1)`). `nan != nan` — kendisiyle
  bile eşit değildir, bu yüzden `==` ile değil `std::isnan()` ile test edilir; aynı
  şekilde `std::isinf()` sonsuzluğu test eder.
- Sondaki **`f`** her zamanki gibi "bu bir `float` literali" anlamına geliyor
  (`nanf`/`+inff`/`-inff`, 4 byte); `f` yoksa `double` (`nan`/`+inf`/`-inf`, 8 byte).
- Bu değerler `char` ve `int`'e **asla** dönüştürülemez — 4/8 byte'lık bu özel bit
  kalıbının 1 veya 4 byte'lık bir tamsayıda hiçbir karşılığı yoktur → `char: impossible`
  ve `int: impossible` basılır, ama `float`/`double` çıktısı `nanf`/`nan`/`inf`/`-inf`
  olarak aynen yazdırılır.
- Kod tarafında bunlar `std::strtof`/`std::strtod` ile parse edilip (bu fonksiyonlar
  metni doğrudan doğru IEEE-754 bit kalıbına çevirir) `std::isnan()`/`std::isinf()` ile
  yakalanıyor (`isSpecial` bayrağı).

### Dönüşüm mantığı

Literal tipi bulunduktan sonra `std::strtol`/`std::strtof`/`std::strtod` ile ham değer
parse edilir, sonra her hedef tipe `static_cast` ile geçilir — yani her seferinde CPU
gerçek bir sayısal dönüştürme yapar (bkz. yukarıdaki `static_cast` bölümü), sadece bit
kopyalamaz. Aralık taşımaları (örn. bir `double`'ın `char`'a veya `int`'e sığmaması)
`impossible` olarak raporlanır. `float`/`double` çıktısında tam sayıya denk geliyorsa
`.0f`/`.0` eki eklenir (subject'in istediği format).

**Çalıştırma:**
```sh
make -C ex00 && ./ex00/convert 42
./ex00/convert nan
./ex00/convert 'a'
```

---

## ex01 — Serializer

Amaç: `reinterpret_cast`'in "bit kalıbı korunur" özelliğini pratikte göstermek.

- `Data` (`ex01/Data.hpp`): `int id` ve `std::string name` içeren sıradan bir struct.
- `Serializer` (`ex01/Serializer.hpp/.cpp`): örneklenemez, iki static fonksiyon:
  - `serialize(Data*) -> uintptr_t`: pointer'ı bir tamsayıya çevirir.
  - `deserialize(uintptr_t) -> Data*`: tamsayıyı geri pointer'a çevirir.

### `main.cpp` sırasında RAM'de tam olarak ne oluyor?

```cpp
Data data;            // <- stack'te ayrılıyor (main fonksiyonunun stack frame'i içinde)
Data *original = &data;  // <- original DA stack'te, ama içindeki DEĞER data'nın stack adresi

uintptr_t raw = Serializer::serialize(original);
// raw = reinterpret_cast<uintptr_t>(original)
// -> hiçbir bit değişmedi, sadece "pointer" etiketi "tamsayı" etiketine döndü
// raw, original ile TAMAMEN AYNI SAYISAL DEĞERİ taşıyan yeni bir stack değişkeni

Data *restored = Serializer::deserialize(raw);
// restored = reinterpret_cast<Data*>(raw)
// -> yine hiçbir bit değişmedi, "tamsayı" etiketi tekrar "pointer" etiketine döndü
```

```
main() fonksiyonunun stack frame'i (yüksekten alçağa doğru adresler):

  0x16d4ce668  ┌─────────────────────────┐
               │ Data data                │  { id = 1337, name = "leet" }
               └─────────────────────────┘
  0x16d4ce660  ┌─────────────────────────┐
               │ Data *original           │  değeri: 0x16d4ce668   (yukarıyı gösteriyor)
               └─────────────────────────┘
  0x16d4ce658  ┌─────────────────────────┐
               │ uintptr_t raw            │  değeri: 0x16d4ce668   (AYNI SAYI, sadece "tamsayı")
               └─────────────────────────┘
  0x16d4ce650  ┌─────────────────────────┐
               │ Data *restored           │  değeri: 0x16d4ce668   (tekrar "pointer")
               └─────────────────────────┘
```

Dikkat: `data`, `original`, `raw`, `restored` değişkenlerinin **hepsi** stack'te
(main'in kendi stack frame'inde) duruyor. Burada **heap yok** — eğer `new Data()`
kullanılsaydı `data` heap'te olur, `original` (heap adresini tutan pointer) yine
stack'te kalırdı. `restored == original` çıkması normal, çünkü ikisi de aynı stack
adresini (aynı sayıyı) taşıyor; `reinterpret_cast` o sayıyı hiç değiştirmedi, sadece
tipini "tamsayı" ↔ "pointer" arasında iki kez değiştirdi.

**Çalıştırma:**
```sh
make -C ex01 && ./ex01/serializer
```

---

## ex02 — Base / A / B / C (dynamic_cast ile tip tanıma)

- `Base`: sadece `virtual ~Base()` içeren soyut olmayan taban sınıf. `virtual` şart,
  çünkü `dynamic_cast` yalnızca **polimorfik** tiplerde çalışır (vtable/RTTI gerektirir,
  bkz. yukarıdaki `dynamic_cast` bölümündeki bellek diyagramı).
- `A`, `B`, `C`: `Base`'den boş türeyen kardeş sınıflar. Her biri kendi vtable'ını
  read-only segmentte taşır (`A`'nın vtable'ı `B`'ninkinden farklı bir RAM adresinde).
- `generate()`: `std::rand() % 3` ile rastgele `A`, `B` ya da `C` döndürür. Nesne
  **`new` ile heap'te** ayrılıyor (dönüş tipi `Base*` — çağıran taraf hangi somut tipi
  aldığını compile-time'da bilmez, sadece `Base*` görür). `main()`'deki
  `Base *base = generate();` satırında `base` pointer'ı **stack'te**, ama gösterdiği
  nesne **heap'te**.
- `identify(Base*)`: sırayla `dynamic_cast<A*>`, `<B*>`, `<C*>` dener; her denemede
  `base`'in vptr'sinden vtable'a, oradan `type_info`'ya gidip karşılaştırma yapılır.
  Başarısız olan `NULL` döner, `if` ile atlanır.
- `identify(Base&)`: aynı RTTI mekanizması ama referans üzerinde — başarısızlıkta
  `NULL` değil **`std::bad_cast` fırlatılır**, bu yüzden her deneme `try/catch` içinde.
- `delete base;`: heap'teki nesneyi serbest bırakır; `Base`'in yıkıcısı `virtual`
  olduğu için doğru türetilmiş yıkıcı (`~A`, `~B` ya da `~C`) çağrılır (vtable
  üzerinden), bellek sızıntısı olmaz.

Bu ikisi `dynamic_cast`'in iki farklı hata modunu (pointer → `NULL`, referans →
exception) yan yana göstermek için var.

**Çalıştırma:**
```sh
make -C ex02 && ./ex02/identify
```

---

## Derleme

Her exercise kendi `Makefile`'ına sahip, hepsi `c++ -Wall -Wextra -Werror -std=c++98`
ile derleniyor. Standart hedefler: `all`, `clean` (`.o` dosyalarını sil), `fclean`
(`clean` + binary'yi sil), `re` (`fclean` + `all`).

```sh
make -C ex00        # convert    üretir
make -C ex01        # serializer üretir
make -C ex02        # identify   üretir

make -C ex00 fclean && make -C ex01 fclean && make -C ex02 fclean   # hepsini temizle
```
