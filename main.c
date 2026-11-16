#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

int main() {
    // Rastgele sayı üreticisini başlatıyoruz
    srand(time(NULL));

    // Değişken tanımlamaları
    int saglik = 100, enerji = 100, oksijen = 100;
    int su = 20, yedekParca = 3, modulHasari = 0;
    int kilitMuhurlu = 0;
    int gun = 1, tur = 0;
    int oyunBitti = 0; // 0: Devam, 1: Kazanma, -1: Kaybetme
    
    char zorlukSecimi;
    int firtinaSuresi = 5;

    // Zorluk Seviyesi Seçimi
    printf("--- MARS KOLONISI YASAM DESTEK SIMULATORU ---\n");
    do {
        printf("Zorluk Seviyesi Seciniz (K: Kolay, N: Normal, Z: Zor): ");
        scanf(" %c", &zorlukSecimi);
        zorlukSecimi = toupper(zorlukSecimi);

        switch (zorlukSecimi) {
            case 'K':
                su = 30; yedekParca = 5; firtinaSuresi = 3;
                printf("Zorluk: Kolay secildi. Baslangic kaynaklari artirildi.\n");
                break;
            case 'N':
                su = 20; yedekParca = 3; firtinaSuresi = 5;
                printf("Zorluk: Normal secildi.\n");
                break;
            case 'Z':
                su = 10; yedekParca = 1; firtinaSuresi = 7;
                modulHasari = 20;
                printf("Zorluk: Zor secildi. Kaynaklar az, baslangic hasari var!\n");
                break;
            default:
                printf("Gecersiz secim! Tekrar deneyin.\n");
                zorlukSecimi = 0; // Tekrar sorması için
        }
    } while (zorlukSecimi == 0);

    // Ana Oyun Döngüsü (do-while)
    do {
        // Her tur başında veya sonunda sınırlandırma kontrolü (0 - 100 arası)
        if (saglik < 0) saglik = 0; if (saglik > 100) saglik = 100;
        if (enerji < 0) enerji = 0; if (enerji > 100) enerji = 100;
        if (oksijen < 0) oksijen = 0; if (oksijen > 100) oksijen = 100;
        if (modulHasari < 0) modulHasari = 0; if (modulHasari > 100) modulHasari = 100;

        // Oyun sonu kontrolleri
        if (saglik == 0 || oksijen == 0) {
            oyunBitti = -1;
            break;
        }
        if (gun > 15) {
            oyunBitti = 1;
            break;
        }

        // Durum Bilgilendirmesi
        printf("\n========================================\n");
        printf("GUN: %d | TUR: %d | Saglik: %d | Enerji: %d | Oksijen: %d\n", gun, tur, saglik, enerji, oksijen);
        printf("Su: %d birim | Yedek Parca: %d | Modul Hasari: %d | Kilit Muhurlu: %d\n", su, yedekParca, modulHasari, kilitMuhurlu);
        printf("========================================\n");
        printf("Komutlar -> [G]esif, [O]ksijen uret, [T]amir, [D]urum raporu, [U]yu, [K]um firtinasi, [H]ava kilidi, [Q] Cikis\n");
        printf("Bir komut secin: ");

        char komut;
        scanf(" %c", &komut);
        komut = toupper(komut);

        int turArttir = 1; // D ve tanımsız komutlar tur harcamaz

        // Komut Sistemi (Switch-Case ve Fall-Through)
        switch (komut) {
            case 'G': { // Keşif Görevi
                if (enerji < 15) {
                    printf("HATA: Kesif icin yeterli enerji yok! (Gereken: 15)\n");
                    turArttir = 0;
                    break;
                }
                enerji -= 15;
                oksijen -= 10;

                int esik = (enerji > 40 && oksijen > 30) ? 60 : 30;
                int zar = rand() % 100;
                printf("Kesif Zari: Zar: %d / Esik: %d -> ", zar, esik);

                if (zar < 10 || modulHasari > 70) {
                    printf("YARALANMA!\n");
                    saglik -= 15;
                } else if (zar < esik) {
                    printf("BASARILI: Su bulundu!\n");
                    su += 5;
                } else if (zar < esik + 20) {
                    printf("BASARILI: Yedek parca bulundu!\n");
                    yedekParca += 1;
                } else {
                    printf("BOS: Eli bos donuldu.\n");
                }
                break;
            }

            case 'O': { // Oksijen Üretimi
                int istenenSu;
                printf("Harcanacak su miktarini girin (Mevcut su: %d): ", su);
                scanf("%d", &istenenSu);

                if (istenenSu <= 0 || istenenSu > su) {
                    printf("HATA: Gecersiz su miktari!\n");
                    turArttir = 0;
                } else {
                    su -= istenenSu;
                    int kalanArtik = istenenSu % 2; // Mod operatörü
                    int kullanilanSu = istenenSu - kalanArtik;
                    su += kalanArtik; // Artık su depoda kalır

                    int uretilenOksijen = (kullanilanSu / 2) * 3;
                    oksijen += uretilenOksijen;
                    printf("Basarili! %d birim su harcandi, %d birim oksijen uretildi. %d birim su artif olarak kaldı.\n", kullanilanSu, uretilenOksijen, kalanArtik);
                }
                break;
            }

            case 'T': { // Tamir
                if (yedekParca <= 0) {
                    printf("HATA: Yedek parcaniz kalmadi! Tamir yapilamaz.\n");
                    turArttir = 0;
                    break;
                }
                yedekParca--;

                // İf-Else If zinciri ile kademeler
                if (modulHasari <= 30) {
                    modulHasari -= 15;
                    printf("Hafif tamir yapildi (-15 Hasar).\n");
                } else if (modulHasari <= 70) {
                    modulHasari -= 25;
                    printf("Orta seviye tamir yapildi (-25 Hasar).\n");
                } else {
                    modulHasari -= 35;
                    printf("Agir seviye tamir yapildi (-35 Hasar).\n");
                }
                break;
            }

            case 'D': { // Durum Raporu
                turArttir = 0; // Tur harcamaz
                printf("\n--- DETAYLI DURUM RAPORU ---\n");
                if (oksijen < 25) printf("KRİTİK UYARI: Oksijen cok dusuk! (<25)\n");
                if (saglik < 30) printf("KRİTİK UYARI: Saglik durumu kritik! (<30)\n");
                if (modulHasari > 70) printf("KRİTİK UYARI: Modul hasari cok yuksek! (>70)\n");

                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70) {
                    printf("TÜM SİSTEMLER NORMAL.\n");
                }
                break;
            }

            case 'U': { // Uyu / Dinlen
                if (oksijen < 20) {
                    printf("UYARI: Oksijen cok dusuk oldugu icin dinlenirken saglik kazanamadiniz, aksine kaybettiniz!\n");
                    saglik -= 10;
                } else {
                    enerji += 20;
                    saglik += 10;
                    oksijen -= 5;
                    printf("Dinlenildi. Enerji ve saglik artti, az miktarda oksijen tuktuldu.\n");
                }
                break;
            }

            case 'K': { // Kum Fırtınası (For Döngüsü)
                printf("Kum fırtınası basladi! Sure: %d tur.\n", firtinaSuresi);
                int toplamSaglikKaybi = 0, toplamOksijenKaybi = 0;

                for (int i = 1; i <= firtinaSuresi; i++) {
                    int zar = rand() % 100;
                    int hasarKatsayisi = (i % 2 == 0) ? 2 : 1; // Çift turlarda rüzgar şiddetlenir (mod kontrolü)

                    if (kilitMuhurlu == 1 && i == 2) {
                        printf("Tur %d: Hava kilidi muhurlu oldugu icin oksijen sizintisi atlandi (continue).\n", i);
                        continue;
                    }

                    if (zar < 30) {
                        int c_can = 5 * hasarKatsayisi;
                        saglik -= c_can;
                        toplamSaglikKaybi += c_can;
                        printf("Tur %d: Fırtınada darbe alindi! Saglik -%d\n", i, c_can);
                    } else if (zar < 60) {
                        int c_oks = 8 * hasarKatsayisi;
                        oksijen -= c_oks;
                        toplamOksijenKaybi += c_oks;
                        printf("Tur %d: Catlaklardan oksijen sizdi! Oksijen -%d\n", i, c_oks);
                    } else {
                        printf("Tur %d: Fırtına hafif atladi.\n", i);
                    }

                    if (saglik <= 0 || oksijen <= 0) {
                        printf("Fırtına sirasinda kritik esige ulasildi, dongu sonlandiriliyor (break).\n");
                        break;
                    }
                }
                printf("Firtina Ozeti: Toplam Saglik Kaybi: %d, Toplam Oksijen Kaybi: %d\n", toplamSaglikKaybi, toplamOksijenKaybi);
                break;
            }

            case 'H': { // Hava Kilidi Şifresi (Do-While Döngüsü)
                char hedefSifre = 'A' + (gun % 26);
                int denemeHakki = 3;
                char girilenHarf;
                int basarili = 0;

                printf("Hava Kilidi Mühürleme Protokolü!\n");
                printf("İpucu: Şifre, alfabede 'A' harfinden bugünün gün sayısı (%d) kadar ilerideki harftir.\n", gun);

                do {
                    printf("Kalan deneme hakki: %d. Bir harf girin (A-Z): ", denemeHakki);
                    scanf(" %c", &girilenHarf);
                    girilenHarf = toupper(girilenHarf);

                    if (girilenHarf == hedefSifre) {
                        basarili = 1;
                        break;
                    } else {
                        denemeHakki--;
                        enerji -= 5;
                        if (girilenHarf < hedefSifre) {
                            printf("Yanlis! Daha İLERİ bir harf deneyin.\n");
                        } else {
                            printf("Yanlis! Daha GERİ bir harf deneyin.\n");
                        }
                    }
                } while (denemeHakki > 0 && girilenHarf != hedefSifre);

                if (basarili) {
                    kilitMuhurlu = 1;
                    printf("Tebrikler! Hava kilidi basariyla muhurlendi.\n");
                } else {
                    modulHasari += 20;
                    printf("Basarisiz! Deneme hakki bitti. Modul hasari 20 artti.\n");
                }
                break;
            }

            case 'Q': // Çıkış
                oyunBitti = -2;
                break;

            default:
                printf("TANIMSIZ KOMUT! Tur harcanmadi.\n");
                turArttir = 0;
                break;
        }

        // Zaman Akışı ve Gün Sayacı Güncellemesi
        if (turArttir && oyunBitti == 0) {
            tur++;
            // Her 3 turda bir yeni gün başlar
            if (tur % 3 == 0) {
                gun++;
                oksijen -= 8;
                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("GÜN BAŞI UYARI: Modul hasarli oldugu icin ek 5 saglik kaybedildi!\n");
                }
                printf(">>> YENİ GÜN BAŞLADI: Gün %d <<<\n", gun);
            }
        }

    } while (oyunBitti == 0);

    // Oyun Sonu Raporu
    printf("\n========================================\n");
    printf("            OYUN SONU RAPORU            \n");
    printf("========================================\n");
    if (oyunBitti == 1) {
        printf("TEBRİKLER! 15. günü tamamladınız, kurtarma ekibi geldi ve kazandınız!\n");
    } else if (oyunBitti == -1) {
        printf("OYUN BİTTİ! Sağlık veya oksijen tükendi, hayatta kalamadınız.\n");
    } else {
        printf("Oyundan çıkış yaptınız.\n");
    }

    // Puan Formülü: Puan = (gun * 100) + saglik + oksijen - modulHasari
    int toplamPuan = (gun * 100) + saglik + oksijen - modulHasari;
    printf("Hayatta Kalınan Gün: %d\n", gun);
    printf("Toplam Tur Sayısı: %d\n", tur);
    printf("Son Durum -> Sağlık: %d, Enerji: %d, Oksijen: %d, Hasar: %d\n", saglik, enerji, oksijen, modulHasari);
    printf("TOPLAM PUANINIZ: %d\n", toplamPuan);
    printf("========================================\n");

    return 0;
}