#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct istatistik
{
    int sure;
    int aboneadet;
    int gorusmeadet;
} Ist;

struct abone
{
    long tel_no;
    char ad[11];
    char soyad[13];
    char adres[36];
    char abone_tipi;
    struct gorusme *gorusme_yapan;
    struct abone *noya_gore_sonraki;
    struct abone *soyada_gore_sonraki;
};

struct gorusme
{
    char aranan_tel_no[11];
    char tarih[10];
    char bas_zamani[5];
    int sure;
    struct gorusme *sonraki;
    struct gorusme *onceki;
};

void ana_menu();
int hash1 (long telefon);
int hash2 (char *soyad);
int bos(struct abone *liste_basi_ptr);
void noya_gore_sirali_ekle(struct abone **liste_basi_ptr, struct abone *yeni);
void gorusme_ekle_ist3(struct abone *bir_abone);
void abone_bilgisi_gorusme_listeleme_ist5(struct abone *bir_abone);
void ist_6(struct abone *bir_abone);
void artan_listele_ist7(struct abone *noya_gore_hash_tablosu);
void alfabetik_listele(struct abone *soyada_gore_hash_tablosu);
void parcaya_gore_listele_ist9(struct abone *soyada_gore_hash_tablosu, char *metin);
void soyada_gore_listele_ist8 (struct abone *soyada_gore_hash_tablosu);
void soyad_sirali_ekle(struct abone *soyada_gore_hash_tablosu, struct abone *yeni_abone);
struct abone *noya_gore_ara(struct abone *liste_basi_ptr,  long telefon);
void tum_abone_bilg_listeleme_ist10 (struct abone *, Ist *);
void ist10(struct abone **noya_gore_hash_tablosu, Ist *istatistik);
void ist_11(struct abone **noya_gore_hash_tablosu, char *tarih);
void ist_12(struct abone *soyada_gore_hash_tablosu, char *telefon);
void ist_13 (struct abone *noya_gore_hash_tablosu, char *tarih);
void no_listesinden_cikar(struct abone **ilk_ptr, struct abone *silinecek, int secim);
void soyad_listesinden_cikar(struct abone *ilk_ptr, struct abone *cikarilacak_ptr);
void gorusme_sil (struct abone *silinecek);

int main()
{
    int secim,i,indeks1,indeks,a=0,indeks3;

    Ist istatistik[5] = {0, 0, 0};

    struct abone *tel_no_hash_tablosu[1000]= {NULL};
    struct abone *onceki, *gecici,*gecici2,*yeni_abone,*bir_abone,soyad_hash_tablosu[26];
    long tel_no;
    char tarih[20],devam,secim2,telefon[20],metin[12],indeks2;

    for(i=0; i<26; i++)
    {
        soyad_hash_tablosu[i].soyada_gore_sonraki=&soyad_hash_tablosu[i]; //liste basi dugumune sahip dairesel listeler ilkleniyor
    }

    while(secim!=14)
    {
        ana_menu();
        scanf("%d",&secim);

        switch(secim)
        {
        case 1:
            printf("\nTelefon numarasini giriniz: ");
            scanf("%ld",&tel_no);
            indeks1=hash1(tel_no);

            if(noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no)==NULL)
            {
                yeni_abone=malloc(sizeof(struct abone));
                yeni_abone->tel_no=tel_no;

                printf("\nAdini giriniz: ");
                scanf("%s",&yeni_abone->ad);
                strupr(yeni_abone->ad); //Ekran ciktilarinin gorunumu icin aldigimiz verileri buyuk harfe cevirdik
                printf("Soyadini giriniz: ");
                scanf("%s",&yeni_abone->soyad);
                strupr(yeni_abone->soyad); //Soyad karsilastirmalarinda hata olmamasi icin ve gorunus guzelligi icin aldigimiz verileri buyuk harfe cevirdik
                printf("Abonenin adresini giriniz: ");
                fflush(stdin);
                gets(yeni_abone->adres);
                strupr(yeni_abone->adres); //Ekran ciktilarinin gorunumu icin aldigimiz verileri buyuk harfe cevirdik
                printf("Abone tipini giriniz (S-I-K-G): ");
                fflush(stdin);
                scanf("%c",&yeni_abone->abone_tipi);

                yeni_abone->gorusme_yapan=malloc(sizeof(struct gorusme)); //Abonenin gorusmeleri icin bellekten alan ayrilir
                yeni_abone->gorusme_yapan->sonraki=yeni_abone->gorusme_yapan;
                yeni_abone->gorusme_yapan->onceki=yeni_abone->gorusme_yapan;

                noya_gore_sirali_ekle(&tel_no_hash_tablosu[indeks1],yeni_abone); //Numaranin ilk 3 hanesine gore ekler
                indeks2=hash2(yeni_abone->soyad);
                soyad_sirali_ekle(&soyad_hash_tablosu[indeks2],yeni_abone); //Soyadin bas harfine gore ekler
            }
            else
            {
                printf("\nBu numaraya ait bir abone bulunmaktadir!\n");
            }
            break ;

        case 2:
            printf("\nAbonenin telefon numarasini giriniz: ");
            scanf("%ld",&tel_no);

            indeks1=hash1(tel_no);
            bir_abone=noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no);

            if(bir_abone==NULL) //Eger abone yoksa
            {
                printf("Bu numaraya sahip bir abone yok!\n");
            }
            else
            {
                printf("\nAbonenin yeni adresini giriniz: ");
                fflush(stdin);
                gets(bir_abone->adres);
                strupr(bir_abone->adres);

                printf("\nAdres guncellenmistir! \nAbonenin telefon numarasini degistirmek ister misiniz? (E,e-H,h):\n ");
                scanf("%c",&secim2);

                if(secim2=='e' || secim2=='E')
                {
                    printf("Abonenin yeni telefon numarasini giriniz: ");
                    scanf("%ld",&tel_no);
                    indeks3=hash1(tel_no);

                    while(noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no)!=NULL)
                    {
                        printf("Abonenin yeni telefon numarasini giriniz: ");
                        scanf("%ld",&tel_no);
                        indeks3=hash1(tel_no);
                    }
                    bir_abone->gorusme_yapan=malloc(sizeof(struct gorusme)); //Abonenin gorusmeleri icin bellekten yer ayrilir
                    bir_abone->gorusme_yapan->sonraki=bir_abone->gorusme_yapan;
                    bir_abone->gorusme_yapan->onceki=bir_abone->gorusme_yapan;
                    no_listesinden_cikar(&tel_no_hash_tablosu[indeks1],bir_abone, 1);
                    bir_abone->tel_no=tel_no;
                    noya_gore_sirali_ekle(&tel_no_hash_tablosu[indeks3],bir_abone);
                }
            }
            break;

        case 3:
            do
            {
                printf("Arayan telefon numarasini giriniz:");
                scanf("%ld", &tel_no);
                indeks1=hash1(tel_no);

                if(noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no)!=NULL) // Eger girilen telefon numarasina sahip abone varsa
                {
                    bir_abone=noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no);
                    gorusme_ekle_ist3(bir_abone);
                }
                else
                    printf("Bu numaraya sahip bir abone bulunmamaktadir!");

                do
                {
                    printf("Baska kayit yapmak ister misiniz?(Cikis icin H giriniz)\n");
                    scanf(" %c", &devam);
                }
                while(devam!='e' && devam!='E' && devam!='h' && devam!='H');
            }
            while(devam=='E' || devam=='e');
            break;
        case 4:
            printf("\nSilinecek abonenin telefon numarasini giriniz: ");
            scanf("%ld",&tel_no);
            indeks1=hash1(tel_no);

            bir_abone=noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no);
            if(bir_abone==NULL) //Abone yoksa
            {
                printf("Bu numaraya sahip bir abone yok!\n");
            }
            else
            {
                indeks2=hash2(bir_abone->soyad);
                no_listesinden_cikar(&tel_no_hash_tablosu[indeks1],bir_abone,0);//Abonenin bilgilerini numaranin ilk 3 hanesine gore tutulan listeden cikarir
                soyad_listesinden_cikar(&soyad_hash_tablosu[indeks2],bir_abone);//Abonenin soyadinin bas harfine gore tutlan listeden abone bilgilerini cikarir
            }
            break;

        case 5:
            printf("\nAbonenin telefon numarasini giriniz: ");
            scanf("%ld",&tel_no);

            indeks1=hash1(tel_no);
            bir_abone=noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no);
            if(bir_abone==NULL)
            {
                printf("Bu numaraya sahip bir abone yok!\n");
            }
            else
            {
                abone_bilgisi_gorusme_listeleme_ist5(bir_abone);
            }
            break;

        case 6:
            printf("\nAbonenin telefon numarasini giriniz: ");
            scanf("%ld",&tel_no);
            indeks1=hash1(tel_no);

            bir_abone=noya_gore_ara(tel_no_hash_tablosu[indeks1],tel_no);
            if(bir_abone==NULL)
            {
                printf("Bu numaraya sahip bir abone yok!\n");
            }
            else
            {
                ist_6(bir_abone);
            }
            break;

        case 7:
            printf("Listelemek istediginiz 3 basamakli sayiyi giriniz:");
            scanf("%d", &indeks);
            artan_listele_ist7(tel_no_hash_tablosu[indeks]);
            break;

        case 8:
            soyada_gore_listele_ist8(soyad_hash_tablosu);
            break;

        case 9:
            printf("Listelemek istediginiz metin parcasini giriniz: ");
            scanf("%s",metin);
            strupr(metin);
            indeks2=hash2(&metin[0]);

            parcaya_gore_listele_ist9(&soyad_hash_tablosu[indeks2],&metin);
            break;

        case 10:
            for(i = 0; i < 5; i++)//Abone tipleri icin tutulan istatistiklere sifir atar
            {
                istatistik[i].aboneadet = 0;
                istatistik[i].gorusmeadet = 0;
                istatistik[i].sure = 0;
            }

            ist10(&tel_no_hash_tablosu, istatistik);

            for(i = 0; i < 4; i++)//Tiplere gore toplama islemi yapar ve gerekli yere atar
            {
                istatistik[4].aboneadet += istatistik[i].aboneadet;
                istatistik[4].gorusmeadet += istatistik[i].gorusmeadet;
                istatistik[4].sure += istatistik[i].sure;
            }

            printf("\nAbone Tipi Abone Sayisi Gorusme Sayisi Toplam Sure Ortalama Sure");
            printf("\n---------- ------------ -------------- ----------- -------------");
            printf("\n%s            %d           %d            %d         %.2f","Sahis",istatistik[0].aboneadet,istatistik[0].gorusmeadet,istatistik[0].sure,(float)istatistik[0].sure/istatistik[0].gorusmeadet);
            printf("\n%s           %d           %d            %d         %.2f","Isyeri",istatistik[1].aboneadet,istatistik[1].gorusmeadet,istatistik[1].sure,(float)istatistik[1].sure/istatistik[1].gorusmeadet);
            printf("\n%s             %d           %d            %d         %.2f","Kamu",istatistik[2].aboneadet,istatistik[2].gorusmeadet,istatistik[2].sure,(float)istatistik[2].sure/istatistik[2].gorusmeadet);
            printf("\n%s         %d           %d            %d         %.2f","Guvenlik",istatistik[3].aboneadet,istatistik[3].gorusmeadet,istatistik[3].sure,(float)istatistik[3].sure/istatistik[3].gorusmeadet);
            printf("\n--------------------------------------------------------------------");
            printf("\n%s           %d           %d            %d         %.2f\n","Toplam",istatistik[4].aboneadet,istatistik[4].gorusmeadet,istatistik[4].sure,(float)istatistik[4].sure/istatistik[4].gorusmeadet);
            break;

        case 11:
            printf("Gorusmeleri listelemek istediginiz tarihi giriniz: ");
            fflush(stdin);
            scanf("%s",&tarih);
            ist_11(&tel_no_hash_tablosu,&tarih);
            break;

        case 12:
            printf("Aranilan telefon numarasini giriniz: ");
            fflush(stdin);
            scanf("%s",&telefon);
            ist_12(soyad_hash_tablosu,&telefon);
            break;

        case 13:
            printf("Gorusmeleri listelemek istediginiz tarihi giriniz: ");
            fflush(stdin);
            scanf("%s",&tarih);

            for(i=100; i<=999; i++) // Hash tablosu bastan sona taraniyor
            {
                if(tel_no_hash_tablosu[i]!=NULL)
                {
                    ist_13(tel_no_hash_tablosu[i],&tarih);
                }
            }
            break;

        case 14:
            break;

        default:
            printf("Gecersiz secim!");
            ana_menu();
            break;
        }
        scanf("d",&secim);
    }
    printf("\nProgram sonlandi.");
    return 0;
}

void ana_menu()
{
    printf("               \nANA MENU\n");
    printf("1-Yeni bir abone ekleme\n");
    printf("2-Abone nakli yapma\n");
    printf("3-Yapilan gorusmelere iliskin bilgi ekleme\n");
    printf("4-Abone kaydi silme\n");
    printf("5-Abonenin bilgilerini ve gorusmelerini listeleme\n");
    printf("6-Abonenin bilgilerini ve son 3 gorusmesini listeleme\n");
    printf("7-Telefon numarasi girilen 3 basamakli sayi ile baslayan aboneleri listeleme\n");
    printf("8-Soyadi belirli bir harf ile baslayan aboneleri listeleme\n");
    printf("9-Soyadi belirli bir metin parcasi ile baslayan aboneleri listeleme\n");
    printf("10-Tum abonelerin bilgilerinin listelenmesi\n");
    printf("11-Girilen tarihten itibaren hic gorusme yapmamis abonelerin listelenmesi\n");
    printf("12-Belirli bir numarayi arayan abonelerin listelenmesi\n");
    printf("13-Belirli bir tarihte yapilan tum gorusmelerin listelenmesi\n");
    printf("14-Cikis\n");
    printf("Lutfen seciminizi giriniz:\n");
}

int hash1 (long telefon)//Abonelerin telefon numaralarinin ilk 3 hanesini bulmamiz için gerekli hash fonksiyonu
{
    return telefon/10000;
}

int hash2 (char *soyad)//Abonelerin soyadlarina gore tutulacak liste icin gerekli hash fonksiyonu
{
    return toupper(soyad[0])-'A';
}

int bos(struct abone *liste_basi_ptr)//Listenin bos olup olmadigini kontrol eder ona gore bir deger dondurur
{
    return liste_basi_ptr==NULL;
}

void noya_gore_sirali_ekle(struct abone **liste_basi_ptr, struct abone *yeni)//Aboneleri hash tablosundaki yerine ekler
{
    struct abone *gecici, *onceki;

    if (*liste_basi_ptr==NULL) //Liste bossa
    {
        yeni->noya_gore_sonraki=NULL;
        (*liste_basi_ptr)=yeni; // Listenin ilk dugumu guncelleniyor
    }
    else if (yeni->tel_no<(*liste_basi_ptr)->tel_no) // Telefon numarasi karsilastirilarak listede eklenecek yer araniyor
    {
        yeni->noya_gore_sonraki=(*liste_basi_ptr);
        (*liste_basi_ptr)=yeni;
    }
    else
    {
        onceki=(*liste_basi_ptr);
        gecici=(*liste_basi_ptr)->noya_gore_sonraki;
        while ((gecici!=NULL) && (gecici->tel_no<yeni->tel_no))
        {
            onceki=gecici;
            gecici=gecici->noya_gore_sonraki;
        }
        yeni->noya_gore_sonraki=gecici;
        onceki->noya_gore_sonraki=yeni;
    }
}

struct abone *noya_gore_ara(struct abone *liste_basi_ptr,  long telefon)//Girilen telefon numarasina gore abone varsa isaretcisini, yoksa NULL dondurur
{
    struct abone *gecici;

    gecici=liste_basi_ptr;
    while (gecici!=NULL && gecici->tel_no<=telefon)
    {
        if (gecici->tel_no==telefon)
            return gecici; // cikarilan abonenin isaretcisi dondurulur
        gecici=gecici->noya_gore_sonraki;
    }
    return NULL;
}

void soyad_sirali_ekle(struct abone *soyada_gore_hash_tablosu, struct abone *yeni_abone)//Aboneleri soyad hash tablosundaki yerine ekler
{
    struct abone *onceki, *gecici;

    onceki=soyada_gore_hash_tablosu;
    gecici=soyada_gore_hash_tablosu->soyada_gore_sonraki;

    strupr(gecici->soyad); // Soyad buyuk harfe cevriliryor
    strupr(yeni_abone->soyad); // Eklenecek abonenin soyadi buyuk harfe cevriliyor, karsilastirmada hata olmamasi icin

    while(gecici!=soyada_gore_hash_tablosu && strcmp(gecici->soyad,yeni_abone->soyad)<0) // Liste sonuna kadar taranirken soyadlar karsilastiriliyor
    {
        onceki=gecici;
        gecici=gecici->soyada_gore_sonraki;
        strupr(gecici->soyad);
    }

    yeni_abone->soyada_gore_sonraki=gecici;
    onceki->soyada_gore_sonraki=yeni_abone;
}

void gorusme_ekle_ist3(struct abone *bir_abone)//Girilen telefon numarasina gore abonenin gorusme_yapan kismina gorusmeleri ekler
{
    long tel_no;
    struct gorusme *bir_gorusme, *gecici;
    int indeks1;
    char devam;

    bir_gorusme=malloc(sizeof(struct gorusme)); // Gorusmeler icin bellekten alan tahsis ediliyor
    printf("Aranan telefon numarasini giriniz:");
    fflush(stdin);
    gets(bir_gorusme->aranan_tel_no);
    printf("Tarihi yil ay gun olarak giriniz:");
    scanf("%s",bir_gorusme->tarih);
    printf("Baslangic saatini ve dakikasini giriniz:");
    scanf("%s",bir_gorusme->bas_zamani);
    printf("Konusma suresini dakika olarak giriniz:");
    scanf("%d", &(bir_gorusme->sure));

    gecici=bir_abone->gorusme_yapan->sonraki;

    while(gecici != bir_abone->gorusme_yapan && strcmp(gecici->tarih, bir_gorusme->tarih) < 0) //Gorusmelerin bulundugu dairesel liste bastan sona taranirken, tarihler karsilastilirak eklenecek yer araniyor
    {
        gecici=gecici->sonraki;
    }

    while(gecici != bir_abone->gorusme_yapan && strcmp(gecici->tarih,bir_gorusme->tarih) == 0 && strcmp(gecici->bas_zamani,bir_gorusme->bas_zamani) < 0) //Eger gorusmelerin tarihi esitse baslangic saatine gore karsilastirma yapilarak yer araniyor
    {
        gecici = gecici->sonraki;
    }

    bir_gorusme->sonraki = gecici;
    bir_gorusme->onceki = gecici->onceki;
    gecici->onceki->sonraki = bir_gorusme;
    gecici->onceki = bir_gorusme;
}
void abone_bilgisi_gorusme_listeleme_ist5(struct abone *bir_abone)//Girilen telefon numarasina gore abone bilgilerini ve gorusmelerini listeler
{
    long tel_no;
    int indeks1,i=0,gorusme_suresi=0;
    float ort_sure=0;
    const char *tip[1];
    struct gorusme *gecici;

    switch(bir_abone->abone_tipi)
    {
    case 'S':
    case 's':
        tip[0] = "SAHIS";
        break;
    case 'I':
    case 'i':
        tip[0] = "ISYERI";
        break;
    case 'K':
    case 'k':
        tip[0] = "KAMU";
        break;
    case 'G':
    case 'g':
        tip[0] = "GUVENLIK";
        break;
    }

    printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
    printf("----------------  ----------  ------------  ---------------------  ----------\n");
    printf("%-16ld  %-10s  %-12s  %-22s %s\n",bir_abone->tel_no,bir_abone->ad,bir_abone->soyad,bir_abone->adres,tip[0]);

    gecici=bir_abone->gorusme_yapan->sonraki;

    while(gecici!=bir_abone->gorusme_yapan) //Gorusmelerin bulundugu dairesel liste tamamen taraniyor
    {
        i++;
        if(i==1) //Gorusme yapilmassa basliklar yazdiriliyor
        {
            printf("\nAranan Tel.Numarasi  Tarih        Baslangic Zamani  Sure\n");
            printf("-------------------  -----------  ----------------  -------\n");
        }
        printf("%.3s-%-16s %.2s.%.2s.%.4s   %.2s.%.2s             %d\n",gecici->aranan_tel_no,&gecici->aranan_tel_no[3],&gecici->tarih[6],&gecici->tarih[4],&gecici->tarih[0],&gecici->bas_zamani[0],&gecici->bas_zamani[2],gecici->sure);
        gorusme_suresi+=gecici->sure;
        gecici=gecici->sonraki;

    }
    if(i==0)
        printf("\nAbonenin gorusme kaydi bulunamadi!\n");

    if(i>0) // 1 veya daha fazla gorusme yapildiysa hesaplamalar yapilip ekrana yazdiriliyor
    {
        ort_sure=(float)gorusme_suresi/i;
        printf("\nToplam gorusme sayisi: %d",i);
        printf("\nToplam gorusme suresi: %d",gorusme_suresi);
        printf("\nOrtalama gorusme suresi: %.2f \n",ort_sure);
    }
}

void ist_6(struct abone *bir_abone)
{
    struct gorusme *gecici;
    const char *tip[1];
    int i=0,a=0;

    switch(bir_abone->abone_tipi)
    {
    case 'S':
    case 's':
        tip[0] = "SAHIS";
        break;
    case 'I':
    case 'i':
        tip[0] = "ISYERI";
        break;
    case 'K':
    case 'k':
        tip[0] = "KAMU";
        break;
    case 'G':
    case 'g':
        tip[0] = "GUVENLIK";
        break;
    }

    printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
    printf("----------------  ----------  ------------  ---------------------  ----------\n");
    printf("%-16ld  %-10s  %-12s  %-22s %s\n",bir_abone->tel_no,bir_abone->ad,bir_abone->soyad,bir_abone->adres,tip[0]);

    gecici=bir_abone->gorusme_yapan->onceki; //geciciye yaoilan en son gorusme ataniyor
    while(gecici!=bir_abone->gorusme_yapan && i<3) //Gorusmeler listesi en fazla 3 gorusme yazdirilacagi icin 3 kere donuyor
    {
        a++;
        if(a==1)
        {
            printf("\nAranan Tel.Numarasi  Tarih        Baslangic Zamani  Sure\n");
            printf("-------------------  -----------  ----------------  -------\n");
        }
        printf("%.3s-%-16s %.2s.%.2s.%.4s   %.2s.%.2s             %d\n",gecici->aranan_tel_no,&gecici->aranan_tel_no[3],&gecici->tarih[6],&gecici->tarih[4],&gecici->tarih[0],&gecici->bas_zamani[0],&gecici->bas_zamani[2],gecici->sure);
        gecici=gecici->onceki; // geciciye bir onceki gorusme ataniyor
        i++;
    }
    if(i==0)
        printf("Abonenin gorusme kaydi bulunamadi!\n");

}

void artan_listele_ist7(struct abone *noya_gore_hash_tablosu)
{
    struct abone *gecici;
    const char *tip[1];

    if(!bos(noya_gore_hash_tablosu))
    {
        gecici=noya_gore_hash_tablosu;

        printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
        printf("----------------  ----------  ------------  ---------------------  ----------\n");

        while (gecici!=NULL)
        {
            switch(gecici->abone_tipi)
            {
            case 'S':
            case 's':
                tip[0] = "SAHIS";
                break;
            case 'I':
            case 'i':
                tip[0] = "ISYERI";
                break;
            case 'K':
            case 'k':
                tip[0] = "KAMU";
                break;
            case 'G':
            case 'g':
                tip[0] = "GUVENLIK";
                break;
            }
            printf("%-16ld  %-10s  %-12s  %-22s %s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,tip[0]);
            gecici=gecici->noya_gore_sonraki;
        }
    }
    else
        printf("Kayit bulunamadi!\n");
}

void soyada_gore_listele_ist8 (struct abone *soyada_gore_hash_tablosu)
{
    char harf;
    int indeks1;
    struct abone *bir_abone;

    printf("Listelemek istediginiz bir harf giriniz: ");
    fflush(stdin);
    scanf("%c",&harf);
    indeks1=toupper(harf)-'A';
    alfabetik_listele(&soyada_gore_hash_tablosu[indeks1]);
}

void alfabetik_listele(struct abone *soyada_gore_hash_tablosu)
{
    struct abone *gecici;
    const char *tip[1];

    if(soyada_gore_hash_tablosu->soyada_gore_sonraki!=soyada_gore_hash_tablosu)
    {
        printf("\nTelefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
        printf("----------------  ----------  ------------  ---------------------  ----------\n");
        gecici=soyada_gore_hash_tablosu->soyada_gore_sonraki;
        while (gecici!=soyada_gore_hash_tablosu)
        {
            switch(gecici->abone_tipi)
            {
            case 'S':
            case 's':
                tip[0] = "SAHIS";
                break;
            case 'I':
            case 'i':
                tip[0] = "ISYERI";
                break;
            case 'K':
            case 'k':
                tip[0] = "KAMU";
                break;
            case 'G':
            case 'g':
                tip[0] = "GUVENLIK";
                break;
            }
            printf("%-16ld  %-10s  %-12s  %-22s %s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,tip[0]);
            gecici=gecici->soyada_gore_sonraki;
        }
    }
    else
        printf("Kayit bulunamadi\n!");
}

void parcaya_gore_listele_ist9(struct abone *soyada_gore_hash_tablosu, char *metin)
{
    int harf_say;
    const char *tip[0];
    struct abone *gecici;
    char *metin2;

    harf_say=strlen(metin); //metinin harf sayisi bulunuyor
    if(soyada_gore_hash_tablosu->soyada_gore_sonraki!=soyada_gore_hash_tablosu)
    {
        printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
        printf("----------------  ----------  ------------  ---------------------  ----------\n");
        gecici=soyada_gore_hash_tablosu->soyada_gore_sonraki;
        while (gecici!=soyada_gore_hash_tablosu) //dairesel listesi bastan sona taranirken, metin parcasi ve soyadlar harf sayisi kadar karsilatiriliyor
        {
            metin2=metin;
            if((strncmp(metin2,gecici->soyad,harf_say))==0)
            {
            switch(gecici->abone_tipi)
            {
            case 'S':
            case 's':
                tip[0] = "SAHIS";
                break;
            case 'I':
            case 'i':
                tip[0] = "ISYERI";
                break;
            case 'K':
            case 'k':
                tip[0] = "KAMU";
                break;
            case 'G':
            case 'g':
                tip[0] = "GUVENLIK";
                break;
            }
            printf("%-16ld  %-10s  %-12s  %-22s %s\n",gecici->tel_no,gecici->ad,gecici->soyad,gecici->adres,tip[0]);
            }
            gecici=gecici->soyada_gore_sonraki;
        }
    }
    else
        printf("Kayit bulunamadi\n!");
}

void ist10(struct abone **noya_gore_hash_tablosu, Ist *istatistik)
{
    int i;
    for(i=100; i<=999; i++) //Hash tablosu bastan sona taraniyor
    {
        tum_abone_bilg_listeleme_ist10(noya_gore_hash_tablosu[i], istatistik);
    }
}

void tum_abone_bilg_listeleme_ist10 (struct abone *noya_gore_hash_tablosu, Ist *istatistik)
{

    struct abone *bir_abone;
    int i;
    struct gorusme *gecici;

    while(noya_gore_hash_tablosu != NULL)
    {
        switch(noya_gore_hash_tablosu->abone_tipi)
        {
        case 'S':
        case 's':
            istatistik[0].aboneadet++;
            gecici=noya_gore_hash_tablosu->gorusme_yapan->sonraki;
            while(gecici!=noya_gore_hash_tablosu->gorusme_yapan)
            {
                istatistik[0].sure += gecici->sure;
                istatistik[0].gorusmeadet++;
                gecici=gecici->sonraki;
            }
            break;

        case 'I':
        case 'i':
            istatistik[1].aboneadet++;
            gecici=noya_gore_hash_tablosu->gorusme_yapan->sonraki;
            while(gecici!=noya_gore_hash_tablosu->gorusme_yapan)
            {
                istatistik[1].sure+=gecici->sure;
                istatistik[1].gorusmeadet++;
                gecici=gecici->sonraki;
            }
            break;

        case 'K':
        case 'k':
            istatistik[2].aboneadet++;
            gecici=noya_gore_hash_tablosu->gorusme_yapan->sonraki;
            while(gecici!=noya_gore_hash_tablosu->gorusme_yapan)
            {
                istatistik[2].sure+=gecici->sure;
                istatistik[2].gorusmeadet++;
                gecici=gecici->sonraki;
            }
            break;

        case 'G':
        case 'g':
            istatistik[3].aboneadet++;
            gecici=noya_gore_hash_tablosu->gorusme_yapan->sonraki;
            while(gecici!=noya_gore_hash_tablosu->gorusme_yapan)
            {
                istatistik[3].sure+=gecici->sure;
                istatistik[3].gorusmeadet++;
                gecici=gecici->sonraki;
            }
            break;
        }
        noya_gore_hash_tablosu=noya_gore_hash_tablosu->noya_gore_sonraki;
    }
}

void ist_11(struct abone **noya_gore_hash_tablosu, char *tarih)
{
    struct gorusme *gecici;
    const char *tip[0];
    int i=0, k=0, j=0;
    struct abone *gecici1;

    for(k=100; k<=999; k++)
    {
        if(noya_gore_hash_tablosu[k]!=NULL)
        {
            gecici1=noya_gore_hash_tablosu[k];
            while(gecici1!=NULL)
            {
                gecici=gecici1->gorusme_yapan->onceki;
                if((strcmp(gecici->tarih,tarih)<0)) //tarihler karsilatiriliyor
                {
                    switch(gecici1->abone_tipi)
                    {
                    case 'S':
                    case 's':
                        tip[0] = "SAHIS";
                        break;
                    case 'I':
                    case 'i':
                        tip[0] = "ISYERI";
                        break;
                    case 'K':
                    case 'k':
                        tip[0] = "KAMU";
                        break;
                    case 'G':
                    case 'g':
                        tip[0] = "GUVENLIK";
                        break;
                    }
                    i++;
                    if(i==1 && j<1) //en az 1 veya daha fazla gorusme varsa, baslıklar yalnizca 1 kere yazdiriliyor
                    {
                        printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
                        printf("----------------  ----------  ------------  ---------------------  ----------\n");
                    }
                    printf("%-16ld  %-10s  %-12s  %-22s %s\n",gecici1->tel_no,gecici1->ad,gecici1->soyad,gecici1->adres,tip[0]);
                }
                gecici1=gecici1->noya_gore_sonraki;
            }
        }
    }
    if(i==0)
        printf("Kayit bulunmamaktadir.\n");
}

void ist_12(struct abone *soyada_gore_hash_tablosu, char *telefon)
{
    struct gorusme *gecici;
    const char *tip[1];
    struct abone *abone_1;
    int i=0,k=0,j=0;

    for(k=0; k<26; k++)
    {

        abone_1=(&soyada_gore_hash_tablosu[k])->soyada_gore_sonraki;

        while(abone_1!= &soyada_gore_hash_tablosu[k])
        {
            gecici=abone_1->gorusme_yapan->sonraki;
            while(gecici!=abone_1->gorusme_yapan) // Dairesel gorusmeler liste tamamen taraniyor
            {
                if((strcmp(gecici->aranan_tel_no,telefon)==0))
                {
                    switch(abone_1->abone_tipi)
                    {
                    case 'S':
                    case 's':
                        tip[0] = "SAHIS";
                        break;
                    case 'I':
                    case 'i':
                        tip[0] = "ISYERI";
                        break;
                    case 'K':
                    case 'k':
                        tip[0] = "KAMU";
                        break;
                    case 'G':
                    case 'g':
                        tip[0] = "GUVENLIK";
                        break;
                    }
                    i++;
                    if(i==1 && j<1)
                    {
                        printf("Telefon Numarasi  Adi         Soyadi        Adresi                 Abone Tipi\n");
                        printf("----------------  ----------  ------------  ---------------------  ----------\n");
                        j++;
                    }
                    printf("%-16ld  %-10s  %-12s  %-22s %s\n",abone_1->tel_no,abone_1->ad,abone_1->soyad,abone_1->adres,tip[0]);
                }
                gecici=gecici->sonraki; // geciciye bir sonraki gorusme ataniyor
            }
            abone_1=abone_1->soyada_gore_sonraki;
        }
    }

    if(i==0)
        printf("Bu numarayi arayan abone bulunmamaktadir.\n");
}

void ist_13 (struct abone *noya_gore_hash_tablosu, char *tarih)
{
    struct gorusme *gecici;
    int i=0;

    while(noya_gore_hash_tablosu != NULL)
    {
        gecici=noya_gore_hash_tablosu->gorusme_yapan->sonraki;
        while(gecici!=noya_gore_hash_tablosu->gorusme_yapan)
        {
            if((strcmp(gecici->tarih,tarih)==0)) // Tarihler esitse if icine giriliyor
            {
                i++;
                if(i==1)
                {
                    printf("\nAranan Tel.Numarasi  Tarih        Baslangic Zamani  Sure\n");
                    printf("-------------------  -----------  ----------------  -------\n");
                }
                printf("%.3s-%-16s %.2s.%.2s.%.4s   %.2s.%.2s             %d\n",gecici->aranan_tel_no,&gecici->aranan_tel_no[3],&gecici->tarih[6],&gecici->tarih[4],&gecici->tarih[0],&gecici->bas_zamani[0],&gecici->bas_zamani[2],gecici->sure);
            }
            gecici=gecici->sonraki; // geciciye bir sonraki gorusme ataniyor
        }
        noya_gore_hash_tablosu=noya_gore_hash_tablosu->noya_gore_sonraki; // bir sonraki aboneye ilerletiliyor
    }
}

void no_listesinden_cikar(struct abone **ilk_ptr, struct abone *silinecek, int secim)
{
    struct abone *onceki, *gecici;

    onceki=(*ilk_ptr);
    if(onceki==silinecek) //cikarilacak dugum ilk dugum ise
    {
        (*ilk_ptr)=(*ilk_ptr)->noya_gore_sonraki;
        if(secim==0)
            free(silinecek);
    }
    else
    {
        gecici=(*ilk_ptr)->noya_gore_sonraki;
        while(gecici!=NULL && gecici->tel_no<silinecek->tel_no)
        {
            onceki=gecici;
            gecici=gecici->noya_gore_sonraki;

        }
        onceki->noya_gore_sonraki=gecici->noya_gore_sonraki;
        if(secim==0)
            free(silinecek);
    }
}

void soyad_listesinden_cikar(struct abone *ilk_ptr, struct abone *cikarilacak_ptr)
{
    struct abone *onceki, *gecici;

    if(ilk_ptr==cikarilacak_ptr) // cikarilacak dugum ilk dugum ise
        ilk_ptr=(ilk_ptr)->soyada_gore_sonraki;
    else
    {
        onceki=ilk_ptr;
        gecici=(ilk_ptr)->soyada_gore_sonraki;
        while(gecici!=cikarilacak_ptr)
        {
            onceki=gecici;
            gecici=gecici->soyada_gore_sonraki;
        }
        onceki->soyada_gore_sonraki=gecici->soyada_gore_sonraki;
    }
}

void gorusme_sil (struct abone *silinecek) //Listeden cikarir ama silmez
{
    silinecek->gorusme_yapan->sonraki=silinecek->gorusme_yapan; //silinecek gorusmenin sonrakisi kendisine baglaniyor
    silinecek->gorusme_yapan->onceki=silinecek->gorusme_yapan; //silinecek gorusmenin oncekisi kendisine baglaniyor
}
