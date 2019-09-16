/ * 
   *   C o p y r i g h t   ( c )   2 0 0 3   B i l i b i l i 
   *   C o p y r i g h t   ( c )   2 0 0 3   F a b r i c e   B e l l a r d 
   *   C o p y r i g h t   ( c )   2 0 1 3 - 2 0 1 5   Z h a n g   R u i   < b b c a l l e n @ g m a i l . c o m > 
   * 
   *   T h i s   f i l e   i s   p a r t   o f   i j k P l a y e r . 
   * 
   *   i j k P l a y e r   i s   f r e e   s o f t w a r e ;   y o u   c a n   r e d i s t r i b u t e   i t   a n d / o r 
   *   m o d i f y   i t   u n d e r   t h e   t e r m s   o f   t h e   G N U   L e s s e r   G e n e r a l   P u b l i c 
   *   L i c e n s e   a s   p u b l i s h e d   b y   t h e   F r e e   S o f t w a r e   F o u n d a t i o n ;   e i t h e r 
   *   v e r s i o n   2 . 1   o f   t h e   L i c e n s e ,   o r   ( a t   y o u r   o p t i o n )   a n y   l a t e r   v e r s i o n . 
   * 
   *   i j k P l a y e r   i s   d i s t r i b u t e d   i n   t h e   h o p e   t h a t   i t   w i l l   b e   u s e f u l , 
   *   b u t   W I T H O U T   A N Y   W A R R A N T Y ;   w i t h o u t   e v e n   t h e   i m p l i e d   w a r r a n t y   o f 
   *   M E R C H A N T A B I L I T Y   o r   F I T N E S S   F O R   A   P A R T I C U L A R   P U R P O S E .     S e e   t h e   G N U 
   *   L e s s e r   G e n e r a l   P u b l i c   L i c e n s e   f o r   m o r e   d e t a i l s . 
   * 
   *   Y o u   s h o u l d   h a v e   r e c e i v e d   a   c o p y   o f   t h e   G N U   L e s s e r   G e n e r a l   P u b l i c 
   *   L i c e n s e   a l o n g   w i t h   i j k P l a y e r ;   i f   n o t ,   w r i t e   t o   t h e   F r e e   S o f t w a r e 
   *   F o u n d a t i o n ,   I n c . ,   5 1   F r a n k l i n   S t r e e t ,   F i f t h   F l o o r ,   B o s t o n ,   M A   0 2 1 1 0 - 1 3 0 1   U S A 
   * / 
 
 # i f n d e f   F F P L A Y _ _ F F _ F F P L A Y _ D E F _ H 
 # d e f i n e   F F P L A Y _ _ F F _ F F P L A Y _ D E F _ H 
 
 / * * 
   *   @ f i l e 
   *   s i m p l e   m e d i a   p l a y e r   b a s e d   o n   t h e   F F m p e g   l i b r a r i e s 
   * / 
 
 # i n c l u d e   " c o n f i g . h " 
 # i n c l u d e   < i n t t y p e s . h > 
 # i n c l u d e   < m a t h . h > 
 # i n c l u d e   < l i m i t s . h > 
 # i n c l u d e   < s i g n a l . h > 
 # i n c l u d e   < s t d i n t . h > 
 
 # i n c l u d e   " l i b a v u t i l / a v s t r i n g . h " 
 # i n c l u d e   " l i b a v u t i l / e v a l . h " 
 # i n c l u d e   " l i b a v u t i l / m a t h e m a t i c s . h " 
 # i n c l u d e   " l i b a v u t i l / p i x d e s c . h " 
 # i n c l u d e   " l i b a v u t i l / i m g u t i l s . h " 
 # i n c l u d e   " l i b a v u t i l / d i c t . h " 
 # i n c l u d e   " l i b a v u t i l / p a r s e u t i l s . h " 
 # i n c l u d e   " l i b a v u t i l / s a m p l e f m t . h " 
 # i n c l u d e   " l i b a v u t i l / a v a s s e r t . h " 
 # i n c l u d e   " l i b a v u t i l / t i m e . h " 
 # i n c l u d e   " l i b a v f o r m a t / a v f o r m a t . h " 
 / /   F F P _ M E R G E :   # i n c l u d e   " l i b a v d e v i c e / a v d e v i c e . h " 
 # i n c l u d e   " l i b s w s c a l e / s w s c a l e . h " 
 # i n c l u d e   " l i b a v u t i l / o p t . h " 
 # i n c l u d e   " l i b a v c o d e c / a v f f t . h " 
 # i n c l u d e   " l i b s w r e s a m p l e / s w r e s a m p l e . h " 
 
 # i f   C O N F I G _ A V F I L T E R 
 #   i n c l u d e   " l i b a v f i l t e r / a v f i l t e r . h " 
 #   i n c l u d e   " l i b a v f i l t e r / b u f f e r s i n k . h " 
 #   i n c l u d e   " l i b a v f i l t e r / b u f f e r s r c . h " 
 # e n d i f 
 # i f   S U P P O R T _ T I N T E R L A C E _ F I L T E R 
 #   i n c l u d e   " l i b a v f i l t e r / a v f i l t e r . h " 
 # e n d i f 
 
 # i n c l u d e   < s t d b o o l . h > 
 # i n c l u d e   " i j k a v f o r m a t / i j k i o m a n a g e r . h " 
 # i n c l u d e   " i j k a v f o r m a t / i j k i o a p p l i c a t i o n . h " 
 # i n c l u d e   " f f _ f f i n c . h " 
 # i n c l u d e   " f f _ f f m s g _ q u e u e . h " 
 # i n c l u d e   " f f _ f f p i p e n o d e . h " 
 # i n c l u d e   " i j k m e t a . h " 
 
 # d e f i n e   D E F A U L T _ H I G H _ W A T E R _ M A R K _ I N _ B Y T E S                 ( 2 5 6   *   1 0 2 4 ) 
 
 / * 
   *   S T A R T :   b u f f e r i n g   a f t e r   p r e p a r e d / s e e k e d 
   *   N E X T :     b u f f e r i n g   f o r   t h e   s e c o n d   t i m e   a f t e r   S T A R T 
   *   M A X :       . . . 
   * / 
 # d e f i n e   D E F A U L T _ F I R S T _ H I G H _ W A T E R _ M A R K _ I N _ M S           ( 1 0 0 ) 
 # d e f i n e   D E F A U L T _ N E X T _ H I G H _ W A T E R _ M A R K _ I N _ M S             ( 1   *   1 0 0 0 ) 
 # d e f i n e   D E F A U L T _ L A S T _ H I G H _ W A T E R _ M A R K _ I N _ M S             ( 5   *   1 0 0 0 ) 
 
 # d e f i n e   B U F F E R I N G _ C H E C K _ P E R _ B Y T E S                               ( 5 1 2 ) 
 # d e f i n e   B U F F E R I N G _ C H E C K _ P E R _ M I L L I S E C O N D S                 ( 5 0 0 ) 
 # d e f i n e   F A S T _ B U F F E R I N G _ C H E C K _ P E R _ M I L L I S E C O N D S       ( 5 0 ) 
 # d e f i n e   M A X _ R E T R Y _ C O N V E R T _ I M A G E                                   ( 3 ) 
 
 # d e f i n e   M A X _ Q U E U E _ S I Z E   ( 1 5   *   1 0 2 4   *   1 0 2 4 ) 
 # d e f i n e   M A X _ A C C U R A T E _ S E E K _ T I M E O U T   ( 5 0 0 0 ) 
 # i f d e f   F F P _ M E R G E 
 # d e f i n e   M I N _ F R A M E S   2 5 
 # e n d i f 
 # d e f i n e   D E F A U L T _ M I N _ F R A M E S     5 0 0 0 0 
 # d e f i n e   M I N _ M I N _ F R A M E S             2 
 # d e f i n e   M A X _ M I N _ F R A M E S             5 0 0 0 0 
 # d e f i n e   M I N _ F R A M E S   ( f f p - > d c c . m i n _ f r a m e s ) 
 # d e f i n e   E X T E R N A L _ C L O C K _ M I N _ F R A M E S   2 
 # d e f i n e   E X T E R N A L _ C L O C K _ M A X _ F R A M E S   1 0 
 
 / *   M i n i m u m   S D L   a u d i o   b u f f e r   s i z e ,   i n   s a m p l e s .   * / 
 # d e f i n e   S D L _ A U D I O _ M I N _ B U F F E R _ S I Z E   5 1 2 
 / *   C a l c u l a t e   a c t u a l   b u f f e r   s i z e   k e e p i n g   i n   m i n d   n o t   c a u s e   t o o   f r e q u e n t   a u d i o   c a l l b a c k s   * / 
 # d e f i n e   S D L _ A U D I O _ M A X _ C A L L B A C K S _ P E R _ S E C   3 0 
 
 / *   S t e p   s i z e   f o r   v o l u m e   c o n t r o l   * / 
 # d e f i n e   S D L _ V O L U M E _ S T E P   ( S D L _ M I X _ M A X V O L U M E   /   5 0 ) 
 
 / *   n o   A V   s y n c   c o r r e c t i o n   i s   d o n e   i f   b e l o w   t h e   m i n i m u m   A V   s y n c   t h r e s h o l d   * / 
 # d e f i n e   A V _ S Y N C _ T H R E S H O L D _ M I N   0 . 0 4 
 / *   A V   s y n c   c o r r e c t i o n   i s   d o n e   i f   a b o v e   t h e   m a x i m u m   A V   s y n c   t h r e s h o l d   * / 
 # d e f i n e   A V _ S Y N C _ T H R E S H O L D _ M A X   0 . 1 
 / *   I f   a   f r a m e   d u r a t i o n   i s   l o n g e r   t h a n   t h i s ,   i t   w i l l   n o t   b e   d u p l i c a t e d   t o   c o m p e n s a t e   A V   s y n c   * / 
 # d e f i n e   A V _ S Y N C _ F R A M E D U P _ T H R E S H O L D   0 . 1 5 
 / *   n o   A V   c o r r e c t i o n   i s   d o n e   i f   t o o   b i g   e r r o r   * / 
 # d e f i n e   A V _ N O S Y N C _ T H R E S H O L D   1 0 0 . 0 
 
 / *   m a x i m u m   a u d i o   s p e e d   c h a n g e   t o   g e t   c o r r e c t   s y n c   * / 
 # d e f i n e   S A M P L E _ C O R R E C T I O N _ P E R C E N T _ M A X   1 0 
 
 / *   e x t e r n a l   c l o c k   s p e e d   a d j u s t m e n t   c o n s t a n t s   f o r   r e a l t i m e   s o u r c e s   b a s e d   o n   b u f f e r   f u l l n e s s   * / 
 # d e f i n e   E X T E R N A L _ C L O C K _ S P E E D _ M I N     0 . 9 0 0 
 # d e f i n e   E X T E R N A L _ C L O C K _ S P E E D _ M A X     1 . 0 1 0 
 # d e f i n e   E X T E R N A L _ C L O C K _ S P E E D _ S T E P   0 . 0 0 1 
 
 / *   w e   u s e   a b o u t   A U D I O _ D I F F _ A V G _ N B   A - V   d i f f e r e n c e s   t o   m a k e   t h e   a v e r a g e   * / 
 # d e f i n e   A U D I O _ D I F F _ A V G _ N B       2 0 
 
 / *   p o l l s   f o r   p o s s i b l e   r e q u i r e d   s c r e e n   r e f r e s h   a t   l e a s t   t h i s   o f t e n ,   s h o u l d   b e   l e s s   t h a n   1 / f p s   * / 
 # d e f i n e   R E F R E S H _ R A T E   0 . 0 1 
 
 / *   N O T E :   t h e   s i z e   m u s t   b e   b i g   e n o u g h   t o   c o m p e n s a t e   t h e   h a r d w a r e   a u d i o   b u f f e r s i z e   s i z e   * / 
 / *   T O D O :   W e   a s s u m e   t h a t   a   d e c o d e d   a n d   r e s a m p l e d   f r a m e   f i t s   i n t o   t h i s   b u f f e r   * / 
 # d e f i n e   S A M P L E _ A R R A Y _ S I Z E   ( 8   *   6 5 5 3 6 ) 
 
 / / a d d e d   b y   y u a n z c   f o r   l i v e   p l a y   a u d i o   d e l a y 
 # d e f i n e   S A M P L E _ M A X _ B U F F E R _ S I Z E   ( 2   *   1 0 2 4 ) 
 / / e n d   a d d e d 
 
 # d e f i n e   M I N _ P K T _ D U R A T I O N   1 5 
 
 # i f d e f   F F P _ M E R G E 
 # d e f i n e   C U R S O R _ H I D E _ D E L A Y   1 0 0 0 0 0 0 
 
 # d e f i n e   U S E _ O N E P A S S _ S U B T I T L E _ R E N D E R   1 
 
 s t a t i c   u n s i g n e d   s w s _ f l a g s   =   S W S _ B I C U B I C ; 
 # e n d i f 
 
 # d e f i n e   H D _ I M A G E   2     / /   6 4 0 * 3 6 0 
 # d e f i n e   S D _ I M A G E   1     / /   3 2 0 * 1 8 0 
 # d e f i n e   L D _ I M A G E   0     / /   1 6 0 * 9 0 
 # d e f i n e   M A X _ D E V I A T I O N   1 2 0 0 0 0 0       / /   1 2 0 0 m s 
 
 t y p e d e f   s t r u c t   G e t I m g I n f o   { 
         c h a r   * i m g _ p a t h ; 
         i n t 6 4 _ t   s t a r t _ t i m e ; 
         i n t 6 4 _ t   e n d _ t i m e ; 
         i n t 6 4 _ t   f r a m e _ i n t e r v a l ; 
         i n t   n u m ; 
         i n t   c o u n t ; 
         i n t   w i d t h ; 
         i n t   h e i g h t ; 
         A V C o d e c C o n t e x t   * f r a m e _ i m g _ c o d e c _ c t x ; 
         s t r u c t   S w s C o n t e x t   * f r a m e _ i m g _ c o n v e r t _ c t x ; 
 }   G e t I m g I n f o ; 
 
 t y p e d e f   s t r u c t   M y A V P a c k e t L i s t   { 
         A V P a c k e t   p k t ; 
         s t r u c t   M y A V P a c k e t L i s t   * n e x t ; 
         i n t   s e r i a l ; 
 }   M y A V P a c k e t L i s t ; 
 
 t y p e d e f   s t r u c t   P a c k e t Q u e u e   { 
         M y A V P a c k e t L i s t   * f i r s t _ p k t ,   * l a s t _ p k t ; 
         i n t   n b _ p a c k e t s ; 
         i n t   s i z e ; 
         i n t 6 4 _ t   d u r a t i o n ; 
         i n t   a b o r t _ r e q u e s t ; 
         i n t   s e r i a l ; 
         S D L _ m u t e x   * m u t e x ; 
         S D L _ c o n d   * c o n d ; 
         M y A V P a c k e t L i s t   * r e c y c l e _ p k t ; 
         i n t   r e c y c l e _ c o u n t ; 
         i n t   a l l o c _ c o u n t ; 
 
         i n t   i s _ b u f f e r _ i n d i c a t o r ; 
 }   P a c k e t Q u e u e ; 
 
 / /   # d e f i n e   V I D E O _ P I C T U R E _ Q U E U E _ S I Z E   3 
 # d e f i n e   V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ M I N                 ( 3 ) 
 # d e f i n e   V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ M A X                 ( 1 6 ) 
 # d e f i n e   V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ D E F A U L T         ( V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ M I N ) 
 # d e f i n e   S U B P I C T U R E _ Q U E U E _ S I Z E   1 6 
 # d e f i n e   S A M P L E _ Q U E U E _ S I Z E   9 
 # d e f i n e   F R A M E _ Q U E U E _ S I Z E   F F M A X ( S A M P L E _ Q U E U E _ S I Z E ,   F F M A X ( V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ M A X ,   S U B P I C T U R E _ Q U E U E _ S I Z E ) ) 
 
 # d e f i n e   V I D E O _ M A X _ F P S _ D E F A U L T   3 0 
 
 t y p e d e f   s t r u c t   A u d i o P a r a m s   { 
         i n t   f r e q ; 
         i n t   c h a n n e l s ; 
         i n t 6 4 _ t   c h a n n e l _ l a y o u t ; 
         e n u m   A V S a m p l e F o r m a t   f m t ; 
         i n t   f r a m e _ s i z e ; 
         i n t   b y t e s _ p e r _ s e c ; 
 }   A u d i o P a r a m s ; 
 
 t y p e d e f   s t r u c t   C l o c k   { 
         d o u b l e   p t s ;                       / *   c l o c k   b a s e   * / 
         d o u b l e   p t s _ d r i f t ;           / *   c l o c k   b a s e   m i n u s   t i m e   a t   w h i c h   w e   u p d a t e d   t h e   c l o c k   * / 
         d o u b l e   l a s t _ u p d a t e d ; 
         d o u b l e   s p e e d ; 
         i n t   s e r i a l ;                       / *   c l o c k   i s   b a s e d   o n   a   p a c k e t   w i t h   t h i s   s e r i a l   * / 
         i n t   p a u s e d ; 
         i n t   * q u e u e _ s e r i a l ;         / *   p o i n t e r   t o   t h e   c u r r e n t   p a c k e t   q u e u e   s e r i a l ,   u s e d   f o r   o b s o l e t e   c l o c k   d e t e c t i o n   * / 
 }   C l o c k ; 
 
 / *   C o m m o n   s t r u c t   f o r   h a n d l i n g   a l l   t y p e s   o f   d e c o d e d   d a t a   a n d   a l l o c a t e d   r e n d e r   b u f f e r s .   * / 
 t y p e d e f   s t r u c t   F r a m e   { 
         A V F r a m e   * f r a m e ; 
         A V S u b t i t l e   s u b ; 
         i n t   s e r i a l ; 
         d o u b l e   p t s ;                       / *   p r e s e n t a t i o n   t i m e s t a m p   f o r   t h e   f r a m e   * / 
         d o u b l e   d u r a t i o n ;             / *   e s t i m a t e d   d u r a t i o n   o f   t h e   f r a m e   * / 
         i n t 6 4 _ t   p o s ;                     / *   b y t e   p o s i t i o n   o f   t h e   f r a m e   i n   t h e   i n p u t   f i l e   * / 
 # i f d e f   F F P _ M E R G E 
         S D L _ T e x t u r e   * b m p ; 
 # e l s e 
         S D L _ V o u t O v e r l a y   * b m p ; 
 # e n d i f 
         i n t   a l l o c a t e d ; 
         i n t   w i d t h ; 
         i n t   h e i g h t ; 
         i n t   f o r m a t ; 
         A V R a t i o n a l   s a r ; 
         i n t   u p l o a d e d ; 
 }   F r a m e ; 
 
 t y p e d e f   s t r u c t   F r a m e Q u e u e   { 
         F r a m e   q u e u e [ F R A M E _ Q U E U E _ S I Z E ] ; 
         i n t   r i n d e x ; 
         i n t   w i n d e x ; 
         i n t   s i z e ; 
         i n t   m a x _ s i z e ; 
         i n t   k e e p _ l a s t ; 
         i n t   r i n d e x _ s h o w n ; 
         S D L _ m u t e x   * m u t e x ; 
         S D L _ c o n d   * c o n d ; 
         P a c k e t Q u e u e   * p k t q ; 
 }   F r a m e Q u e u e ; 
 
 e n u m   { 
         A V _ S Y N C _ A U D I O _ M A S T E R ,   / *   d e f a u l t   c h o i c e   * / 
         A V _ S Y N C _ V I D E O _ M A S T E R , 
         A V _ S Y N C _ E X T E R N A L _ C L O C K ,   / *   s y n c h r o n i z e   t o   a n   e x t e r n a l   c l o c k   * / 
 } ; 
 
 t y p e d e f   s t r u c t   D e c o d e r   { 
         A V P a c k e t   p k t ; 
         A V P a c k e t   p k t _ t e m p ; 
         P a c k e t Q u e u e   * q u e u e ; 
         A V C o d e c C o n t e x t   * a v c t x ; 
         i n t   p k t _ s e r i a l ; 
         i n t   f i n i s h e d ; 
         i n t   p a c k e t _ p e n d i n g ; 
         i n t   b f s c _ r e t ; 
         u i n t 8 _ t   * b f s c _ d a t a ; 
 
         S D L _ c o n d   * e m p t y _ q u e u e _ c o n d ; 
         i n t 6 4 _ t   s t a r t _ p t s ; 
         A V R a t i o n a l   s t a r t _ p t s _ t b ; 
         i n t 6 4 _ t   n e x t _ p t s ; 
         A V R a t i o n a l   n e x t _ p t s _ t b ; 
         S D L _ T h r e a d   * d e c o d e r _ t i d ; 
         S D L _ T h r e a d   _ d e c o d e r _ t i d ; 
 
         S D L _ P r o f i l e r   d e c o d e _ p r o f i l e r ; 
         U i n t 6 4   f i r s t _ f r a m e _ d e c o d e d _ t i m e ; 
         i n t         f i r s t _ f r a m e _ d e c o d e d ; 
 }   D e c o d e r ; 
 
 t y p e d e f   s t r u c t   V i d e o S t a t e   { 
         S D L _ T h r e a d   * r e a d _ t i d ; 
         S D L _ T h r e a d   _ r e a d _ t i d ; 
         A V I n p u t F o r m a t   * i f o r m a t ; 
         i n t   a b o r t _ r e q u e s t ; 
         i n t   f o r c e _ r e f r e s h ; 
         i n t   p a u s e d ; 
         i n t   l a s t _ p a u s e d ; 
         i n t   q u e u e _ a t t a c h m e n t s _ r e q ; 
         i n t   s e e k _ r e q ; 
         i n t   s e e k _ f l a g s ; 
         i n t 6 4 _ t   s e e k _ p o s ; 
         i n t 6 4 _ t   s e e k _ r e l ; 
 # i f d e f   F F P _ M E R G E 
         i n t   r e a d _ p a u s e _ r e t u r n ; 
 # e n d i f 
         A V F o r m a t C o n t e x t   * i c ; 
         i n t   r e a l t i m e ; 
 
         C l o c k   a u d c l k ; 
         C l o c k   v i d c l k ; 
         C l o c k   e x t c l k ; 
 
         F r a m e Q u e u e   p i c t q ; 
         F r a m e Q u e u e   s u b p q ; 
         F r a m e Q u e u e   s a m p q ; 
 
         D e c o d e r   a u d d e c ; 
         D e c o d e r   v i d d e c ; 
         D e c o d e r   s u b d e c ; 
 
         i n t   a u d i o _ s t r e a m ; 
 
         i n t   a v _ s y n c _ t y p e ; 
         v o i d   * h a n d l e ; 
         d o u b l e   a u d i o _ c l o c k ; 
         i n t   a u d i o _ c l o c k _ s e r i a l ; 
         d o u b l e   a u d i o _ d i f f _ c u m ;   / *   u s e d   f o r   A V   d i f f e r e n c e   a v e r a g e   c o m p u t a t i o n   * / 
         d o u b l e   a u d i o _ d i f f _ a v g _ c o e f ; 
         d o u b l e   a u d i o _ d i f f _ t h r e s h o l d ; 
         i n t   a u d i o _ d i f f _ a v g _ c o u n t ; 
         A V S t r e a m   * a u d i o _ s t ; 
         P a c k e t Q u e u e   a u d i o q ; 
         i n t   a u d i o _ h w _ b u f _ s i z e ; 
         u i n t 8 _ t   * a u d i o _ b u f ; 
         u i n t 8 _ t   * a u d i o _ b u f 1 ; 
         s h o r t   * a u d i o _ n e w _ b u f ;     / *   f o r   s o u n d t o u c h   b u f   * / 
         u n s i g n e d   i n t   a u d i o _ b u f _ s i z e ;   / *   i n   b y t e s   * / 
         u n s i g n e d   i n t   a u d i o _ b u f 1 _ s i z e ; 
         u n s i g n e d   i n t   a u d i o _ n e w _ b u f _ s i z e ; 
         i n t   a u d i o _ b u f _ i n d e x ;   / *   i n   b y t e s   * / 
         i n t   a u d i o _ w r i t e _ b u f _ s i z e ; 
         i n t   a u d i o _ v o l u m e ; 
         i n t   m u t e d ; 
         s t r u c t   A u d i o P a r a m s   a u d i o _ s r c ; 
 # i f   C O N F I G _ A V F I L T E R 
         s t r u c t   A u d i o P a r a m s   a u d i o _ f i l t e r _ s r c ; 
 # e n d i f 
         s t r u c t   A u d i o P a r a m s   a u d i o _ t g t ; 
         s t r u c t   S w r C o n t e x t   * s w r _ c t x ; 
         i n t   f r a m e _ d r o p s _ e a r l y ; 
         i n t   f r a m e _ d r o p s _ l a t e ; 
         i n t   c o n t i n u o u s _ f r a m e _ d r o p s _ e a r l y ; 
 
         e n u m   S h o w M o d e   { 
                 S H O W _ M O D E _ N O N E   =   - 1 ,   S H O W _ M O D E _ V I D E O   =   0 ,   S H O W _ M O D E _ W A V E S ,   S H O W _ M O D E _ R D F T ,   S H O W _ M O D E _ N B 
         }   s h o w _ m o d e ; 
         i n t 1 6 _ t   s a m p l e _ a r r a y [ S A M P L E _ A R R A Y _ S I Z E ] ; 
         i n t   s a m p l e _ a r r a y _ i n d e x ; 
         i n t   l a s t _ i _ s t a r t ; 
 # i f d e f   F F P _ M E R G E 
         R D F T C o n t e x t   * r d f t ; 
         i n t   r d f t _ b i t s ; 
         F F T S a m p l e   * r d f t _ d a t a ; 
         i n t   x p o s ; 
 # e n d i f 
         d o u b l e   l a s t _ v i s _ t i m e ; 
 # i f d e f   F F P _ M E R G E 
         S D L _ T e x t u r e   * v i s _ t e x t u r e ; 
         S D L _ T e x t u r e   * s u b _ t e x t u r e ; 
 # e n d i f 
 
         i n t   s u b t i t l e _ s t r e a m ; 
         A V S t r e a m   * s u b t i t l e _ s t ; 
         P a c k e t Q u e u e   s u b t i t l e q ; 
 
         d o u b l e   f r a m e _ t i m e r ; 
         d o u b l e   f r a m e _ l a s t _ r e t u r n e d _ t i m e ; 
         d o u b l e   f r a m e _ l a s t _ f i l t e r _ d e l a y ; 
         i n t   v i d e o _ s t r e a m ; 
         A V S t r e a m   * v i d e o _ s t ; 
         P a c k e t Q u e u e   v i d e o q ; 
         d o u b l e   m a x _ f r a m e _ d u r a t i o n ;             / /   m a x i m u m   d u r a t i o n   o f   a   f r a m e   -   a b o v e   t h i s ,   w e   c o n s i d e r   t h e   j u m p   a   t i m e s t a m p   d i s c o n t i n u i t y 
         s t r u c t   S w s C o n t e x t   * i m g _ c o n v e r t _ c t x ; 
 # i f d e f   F F P _ S U B 
         s t r u c t   S w s C o n t e x t   * s u b _ c o n v e r t _ c t x ; 
 # e n d i f 
         i n t   e o f ; 
 
         c h a r   * f i l e n a m e ; 
         i n t   w i d t h ,   h e i g h t ,   x l e f t ,   y t o p ; 
         i n t   s t e p ; 
 
 # i f   C O N F I G _ A V F I L T E R 
         i n t   v f i l t e r _ i d x ; 
         A V F i l t e r C o n t e x t   * i n _ v i d e o _ f i l t e r ;       / /   t h e   f i r s t   f i l t e r   i n   t h e   v i d e o   c h a i n 
         A V F i l t e r C o n t e x t   * o u t _ v i d e o _ f i l t e r ;     / /   t h e   l a s t   f i l t e r   i n   t h e   v i d e o   c h a i n 
         A V F i l t e r C o n t e x t   * i n _ a u d i o _ f i l t e r ;       / /   t h e   f i r s t   f i l t e r   i n   t h e   a u d i o   c h a i n 
         A V F i l t e r C o n t e x t   * o u t _ a u d i o _ f i l t e r ;     / /   t h e   l a s t   f i l t e r   i n   t h e   a u d i o   c h a i n 
         A V F i l t e r G r a p h   * a g r a p h ;                             / /   a u d i o   f i l t e r   g r a p h 
 # e n d i f 
         i n t   l a s t _ v i d e o _ s t r e a m ,   l a s t _ a u d i o _ s t r e a m ,   l a s t _ s u b t i t l e _ s t r e a m ; 
 
         S D L _ c o n d   * c o n t i n u e _ r e a d _ t h r e a d ; 
 
         / *   e x t r a   f i e l d s   * / 
         S D L _ m u t e x     * p l a y _ m u t e x ;   / /   o n l y   g u a r d   s t a t e ,   d o   n o t   b l o c k   a n y   l o n g   o p e r a t i o n 
         S D L _ T h r e a d   * v i d e o _ r e f r e s h _ t i d ; 
         S D L _ T h r e a d   _ v i d e o _ r e f r e s h _ t i d ; 
 
         i n t   b u f f e r i n g _ o n ; 
         i n t   p a u s e _ r e q ; 
 
         i n t   d r o p p i n g _ f r a m e ; 
         i n t   i s _ v i d e o _ h i g h _ f p s ;   / /   a b o v e   3 0 f p s 
         i n t   i s _ v i d e o _ h i g h _ r e s ;   / /   a b o v e   1 0 8 0 p 
 
         P a c k e t Q u e u e   * b u f f e r _ i n d i c a t o r _ q u e u e ; 
 
         v o l a t i l e   i n t   l a t e s t _ v i d e o _ s e e k _ l o a d _ s e r i a l ; 
         v o l a t i l e   i n t   l a t e s t _ a u d i o _ s e e k _ l o a d _ s e r i a l ; 
         v o l a t i l e   i n t 6 4 _ t   l a t e s t _ s e e k _ l o a d _ s t a r t _ a t ; 
 
         i n t   d r o p _ a f r a m e _ c o u n t ; 
         i n t   d r o p _ v f r a m e _ c o u n t ; 
         i n t 6 4 _ t   a c c u r a t e _ s e e k _ s t a r t _ t i m e ; 
         v o l a t i l e   i n t 6 4 _ t   a c c u r a t e _ s e e k _ v f r a m e _ p t s ; 
         v o l a t i l e   i n t 6 4 _ t   a c c u r a t e _ s e e k _ a f r a m e _ p t s ; 
         i n t   a u d i o _ a c c u r a t e _ s e e k _ r e q ; 
         i n t   v i d e o _ a c c u r a t e _ s e e k _ r e q ; 
         S D L _ m u t e x   * a c c u r a t e _ s e e k _ m u t e x ; 
         S D L _ c o n d     * v i d e o _ a c c u r a t e _ s e e k _ c o n d ; 
         S D L _ c o n d     * a u d i o _ a c c u r a t e _ s e e k _ c o n d ; 
         v o l a t i l e   i n t   i n i t i a l i z e d _ d e c o d e r ; 
         i n t   s e e k _ b u f f e r i n g ; 
 }   V i d e o S t a t e ; 
 
 / *   o p t i o n s   s p e c i f i e d   b y   t h e   u s e r   * / 
 # i f d e f   F F P _ M E R G E 
 s t a t i c   A V I n p u t F o r m a t   * f i l e _ i f o r m a t ; 
 s t a t i c   c o n s t   c h a r   * i n p u t _ f i l e n a m e ; 
 s t a t i c   c o n s t   c h a r   * w i n d o w _ t i t l e ; 
 s t a t i c   i n t   d e f a u l t _ w i d t h     =   6 4 0 ; 
 s t a t i c   i n t   d e f a u l t _ h e i g h t   =   4 8 0 ; 
 s t a t i c   i n t   s c r e e n _ w i d t h     =   0 ; 
 s t a t i c   i n t   s c r e e n _ h e i g h t   =   0 ; 
 s t a t i c   i n t   a u d i o _ d i s a b l e ; 
 s t a t i c   i n t   v i d e o _ d i s a b l e ; 
 s t a t i c   i n t   s u b t i t l e _ d i s a b l e ; 
 s t a t i c   c o n s t   c h a r *   w a n t e d _ s t r e a m _ s p e c [ A V M E D I A _ T Y P E _ N B ]   =   { 0 } ; 
 s t a t i c   i n t   s e e k _ b y _ b y t e s   =   - 1 ; 
 s t a t i c   i n t   d i s p l a y _ d i s a b l e ; 
 s t a t i c   i n t   s h o w _ s t a t u s   =   1 ; 
 s t a t i c   i n t   a v _ s y n c _ t y p e   =   A V _ S Y N C _ A U D I O _ M A S T E R ; 
 s t a t i c   i n t 6 4 _ t   s t a r t _ t i m e   =   A V _ N O P T S _ V A L U E ; 
 s t a t i c   i n t 6 4 _ t   d u r a t i o n   =   A V _ N O P T S _ V A L U E ; 
 s t a t i c   i n t   f a s t   =   0 ; 
 s t a t i c   i n t   g e n p t s   =   0 ; 
 s t a t i c   i n t   l o w r e s   =   0 ; 
 s t a t i c   i n t   d e c o d e r _ r e o r d e r _ p t s   =   - 1 ; 
 s t a t i c   i n t   a u t o e x i t ; 
 s t a t i c   i n t   e x i t _ o n _ k e y d o w n ; 
 s t a t i c   i n t   e x i t _ o n _ m o u s e d o w n ; 
 s t a t i c   i n t   l o o p   =   1 ; 
 s t a t i c   i n t   f r a m e d r o p   =   - 1 ; 
 s t a t i c   i n t   i n f i n i t e _ b u f f e r   =   - 1 ; 
 s t a t i c   e n u m   S h o w M o d e   s h o w _ m o d e   =   S H O W _ M O D E _ N O N E ; 
 s t a t i c   c o n s t   c h a r   * a u d i o _ c o d e c _ n a m e ; 
 s t a t i c   c o n s t   c h a r   * s u b t i t l e _ c o d e c _ n a m e ; 
 s t a t i c   c o n s t   c h a r   * v i d e o _ c o d e c _ n a m e ; 
 d o u b l e   r d f t s p e e d   =   0 . 0 2 ; 
 s t a t i c   i n t 6 4 _ t   c u r s o r _ l a s t _ s h o w n ; 
 s t a t i c   i n t   c u r s o r _ h i d d e n   =   0 ; 
 # i f   C O N F I G _ A V F I L T E R 
 s t a t i c   c o n s t   c h a r   * * v f i l t e r s _ l i s t   =   N U L L ; 
 s t a t i c   i n t   n b _ v f i l t e r s   =   0 ; 
 s t a t i c   c h a r   * a f i l t e r s   =   N U L L ; 
 # e n d i f 
 s t a t i c   i n t   a u t o r o t a t e   =   1 ; 
 s t a t i c   i n t   f i n d _ s t r e a m _ i n f o   =   1 ; 
 
 / *   c u r r e n t   c o n t e x t   * / 
 s t a t i c   i n t   i s _ f u l l _ s c r e e n ; 
 s t a t i c   i n t 6 4 _ t   a u d i o _ c a l l b a c k _ t i m e ; 
 
 s t a t i c   A V P a c k e t   f l u s h _ p k t ; 
 s t a t i c   A V P a c k e t   e o f _ p k t ; 
 
 # d e f i n e   F F _ A L L O C _ E V E N T       ( S D L _ U S E R E V E N T ) 
 # d e f i n e   F F _ Q U I T _ E V E N T         ( S D L _ U S E R E V E N T   +   2 ) 
 
 s t a t i c   S D L _ W i n d o w   * w i n d o w ; 
 s t a t i c   S D L _ R e n d e r e r   * r e n d e r e r ; 
 # e n d i f 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   *   e n d   a t   l i n e   3 3 0   i n   f f p l a y . c 
   *   n e a r   p a c k e t _ q u e u e _ p u t 
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 t y p e d e f   s t r u c t   F F T r a c k C a c h e S t a t i s t i c 
 { 
         i n t 6 4 _ t   d u r a t i o n ; 
         i n t 6 4 _ t   b y t e s ; 
         i n t 6 4 _ t   p a c k e t s ; 
 }   F F T r a c k C a c h e S t a t i s t i c ; 
 
 t y p e d e f   s t r u c t   F F S t a t i s t i c 
 { 
         i n t 6 4 _ t   v d e c _ t y p e ; 
 
         f l o a t   v f p s ; 
         f l o a t   v d p s ; 
         f l o a t   a v d e l a y ; 
         f l o a t   a v d i f f ; 
         i n t 6 4 _ t   b i t _ r a t e ; 
 
         F F T r a c k C a c h e S t a t i s t i c   v i d e o _ c a c h e ; 
         F F T r a c k C a c h e S t a t i s t i c   a u d i o _ c a c h e ; 
 
         i n t 6 4 _ t   b u f _ b a c k w a r d s ; 
         i n t 6 4 _ t   b u f _ f o r w a r d s ; 
         i n t 6 4 _ t   b u f _ c a p a c i t y ; 
         S D L _ S p e e d S a m p l e r 2   t c p _ r e a d _ s a m p l e r ; 
         i n t 6 4 _ t   l a t e s t _ s e e k _ l o a d _ d u r a t i o n ; 
         i n t 6 4 _ t   b y t e _ c o u n t ; 
         i n t 6 4 _ t   c a c h e _ p h y s i c a l _ p o s ; 
         i n t 6 4 _ t   c a c h e _ f i l e _ f o r w a r d s ; 
         i n t 6 4 _ t   c a c h e _ f i l e _ p o s ; 
         i n t 6 4 _ t   c a c h e _ c o u n t _ b y t e s ; 
         i n t 6 4 _ t   l o g i c a l _ f i l e _ s i z e ; 
         i n t   d r o p _ f r a m e _ c o u n t ; 
         i n t   d e c o d e _ f r a m e _ c o u n t ; 
         f l o a t   d r o p _ f r a m e _ r a t e ; 
 }   F F S t a t i s t i c ; 
 
 # d e f i n e   F F P _ T C P _ R E A D _ S A M P L E _ R A N G E   2 0 0 0 
 i n l i n e   s t a t i c   v o i d   f f p _ r e s e t _ s t a t i s t i c ( F F S t a t i s t i c   * d c c ) 
 { 
         m e m s e t ( d c c ,   0 ,   s i z e o f ( F F S t a t i s t i c ) ) ; 
         S D L _ S p e e d S a m p l e r 2 R e s e t ( & d c c - > t c p _ r e a d _ s a m p l e r ,   F F P _ T C P _ R E A D _ S A M P L E _ R A N G E ) ; 
 } 
 
 t y p e d e f   s t r u c t   F F D e m u x C a c h e C o n t r o l 
 { 
         i n t   m i n _ f r a m e s ; 
         i n t   m a x _ b u f f e r _ s i z e ; 
         i n t   h i g h _ w a t e r _ m a r k _ i n _ b y t e s ; 
 
         i n t   f i r s t _ h i g h _ w a t e r _ m a r k _ i n _ m s ; 
         i n t   n e x t _ h i g h _ w a t e r _ m a r k _ i n _ m s ; 
         i n t   l a s t _ h i g h _ w a t e r _ m a r k _ i n _ m s ; 
         i n t   c u r r e n t _ h i g h _ w a t e r _ m a r k _ i n _ m s ; 
 }   F F D e m u x C a c h e C o n t r o l ; 
 
 i n l i n e   s t a t i c   v o i d   f f p _ r e s e t _ d e m u x _ c a c h e _ c o n t r o l ( F F D e m u x C a c h e C o n t r o l   * d c c ) 
 { 
         d c c - > m i n _ f r a m e s                                 =   D E F A U L T _ M I N _ F R A M E S ; 
         d c c - > m a x _ b u f f e r _ s i z e                       =   M A X _ Q U E U E _ S I Z E ; 
         d c c - > h i g h _ w a t e r _ m a r k _ i n _ b y t e s     =   D E F A U L T _ H I G H _ W A T E R _ M A R K _ I N _ B Y T E S ; 
 
         d c c - > f i r s t _ h i g h _ w a t e r _ m a r k _ i n _ m s         =   D E F A U L T _ F I R S T _ H I G H _ W A T E R _ M A R K _ I N _ M S ; 
         d c c - > n e x t _ h i g h _ w a t e r _ m a r k _ i n _ m s           =   D E F A U L T _ N E X T _ H I G H _ W A T E R _ M A R K _ I N _ M S ; 
         d c c - > l a s t _ h i g h _ w a t e r _ m a r k _ i n _ m s           =   D E F A U L T _ L A S T _ H I G H _ W A T E R _ M A R K _ I N _ M S ; 
         d c c - > c u r r e n t _ h i g h _ w a t e r _ m a r k _ i n _ m s     =   D E F A U L T _ F I R S T _ H I G H _ W A T E R _ M A R K _ I N _ M S ; 
 } 
 
 e n u m   A V R e c o r d S t a t e   { 
         A V R E C O R D _ S T A T E _ U N K N O W N   =   0 ,     
         A V R E C O R D _ S T A T E _ I N I T , 
         A V R E C O R D _ S T A T E _ R E C O R D I N G , 
         A V R E C O R D _ S T A T E _ U N I N I T 
 } ; 
 
 / / a d d   b y   y u a n z c 
 e n u m   A V P l a y T y p e   { 
         A V P L A Y _ L I V E _ N O R M A L   =   0 ,     
         A V P L A Y _ P L A Y B A C K _ N O R M A L , 
         A V P L A Y _ L I V E _ I N T E R L A C E , 
         A V P L A Y _ P L A Y B A C K _ I N T E R L A C E , 
 } ; 
 
 
 / *   f f p l a y e r   * / 
 s t r u c t   I j k M e d i a M e t a ; 
 s t r u c t   I J K F F _ P i p e l i n e ; 
 t y p e d e f   s t r u c t   F F P l a y e r   { 
         c o n s t   A V C l a s s   * a v _ c l a s s ; 
 
         / *   f f p l a y   c o n t e x t   * / 
         V i d e o S t a t e   * i s ; 
 
         / *   f o r m a t / c o d e c   o p t i o n s   * / 
         A V D i c t i o n a r y   * f o r m a t _ o p t s ; 
         A V D i c t i o n a r y   * c o d e c _ o p t s ; 
         A V D i c t i o n a r y   * s w s _ d i c t ; 
         A V D i c t i o n a r y   * p l a y e r _ o p t s ; 
         A V D i c t i o n a r y   * s w r _ o p t s ; 
         A V D i c t i o n a r y   * s w r _ p r e s e t _ o p t s ; 
 
         / *   f f p l a y   o p t i o n s   s p e c i f i e d   b y   t h e   u s e r   * / 
 # i f d e f   F F P _ M E R G E 
         A V I n p u t F o r m a t   * f i l e _ i f o r m a t ; 
 # e n d i f 
         c h a r   * i n p u t _ f i l e n a m e ; 
 # i f d e f   F F P _ M E R G E 
         c o n s t   c h a r   * w i n d o w _ t i t l e ; 
         i n t   f s _ s c r e e n _ w i d t h ; 
         i n t   f s _ s c r e e n _ h e i g h t ; 
         i n t   d e f a u l t _ w i d t h ; 
         i n t   d e f a u l t _ h e i g h t ; 
         i n t   s c r e e n _ w i d t h ; 
         i n t   s c r e e n _ h e i g h t ; 
 # e n d i f 
         i n t   a u d i o _ d i s a b l e ; 
         i n t   v i d e o _ d i s a b l e ; 
         i n t   s u b t i t l e _ d i s a b l e ; 
         c o n s t   c h a r *   w a n t e d _ s t r e a m _ s p e c [ A V M E D I A _ T Y P E _ N B ] ; 
         i n t   s e e k _ b y _ b y t e s ; 
         i n t   d i s p l a y _ d i s a b l e ; 
         i n t   s h o w _ s t a t u s ; 
         i n t   a v _ s y n c _ t y p e ; 
         i n t 6 4 _ t   s t a r t _ t i m e ; 
         i n t 6 4 _ t   d u r a t i o n ; 
         i n t   f a s t ; 
         i n t   g e n p t s ; 
         i n t   l o w r e s ; 
         i n t   d e c o d e r _ r e o r d e r _ p t s ; 
         i n t   a u t o e x i t ; 
 # i f d e f   F F P _ M E R G E 
         i n t   e x i t _ o n _ k e y d o w n ; 
         i n t   e x i t _ o n _ m o u s e d o w n ; 
 # e n d i f 
         i n t   l o o p ; 
         i n t   f r a m e d r o p ; 
         i n t 6 4 _ t   s e e k _ a t _ s t a r t ; 
         i n t   s u b t i t l e ; 
         i n t   i n f i n i t e _ b u f f e r ; 
         e n u m   S h o w M o d e   s h o w _ m o d e ; 
         c h a r   * a u d i o _ c o d e c _ n a m e ; 
         c h a r   * s u b t i t l e _ c o d e c _ n a m e ; 
         c h a r   * v i d e o _ c o d e c _ n a m e ; 
         d o u b l e   r d f t s p e e d ; 
 # i f d e f   F F P _ M E R G E 
         i n t 6 4 _ t   c u r s o r _ l a s t _ s h o w n ; 
         i n t   c u r s o r _ h i d d e n ; 
 # e n d i f 
 # i f   C O N F I G _ A V F I L T E R 
         c o n s t   c h a r   * * v f i l t e r s _ l i s t ; 
         i n t   n b _ v f i l t e r s ; 
         c h a r   * a f i l t e r s ; 
         c h a r   * v f i l t e r 0 ; 
 # e n d i f 
         i n t   a u t o r o t a t e ; 
         i n t   f i n d _ s t r e a m _ i n f o ; 
         u n s i g n e d   s w s _ f l a g s ; 
 
         / *   c u r r e n t   c o n t e x t   * / 
 # i f d e f   F F P _ M E R G E 
         i n t   i s _ f u l l _ s c r e e n ; 
 # e n d i f 
         i n t 6 4 _ t   a u d i o _ c a l l b a c k _ t i m e ; 
 # i f d e f   F F P _ M E R G E 
         S D L _ S u r f a c e   * s c r e e n ; 
 # e n d i f 
 
         / *   e x t r a   f i e l d s   * / 
         S D L _ A o u t   * a o u t ; 
         S D L _ V o u t   * v o u t ; 
         s t r u c t   I J K F F _ P i p e l i n e   * p i p e l i n e ; 
         s t r u c t   I J K F F _ P i p e n o d e   * n o d e _ v d e c ; 
         i n t   s a r _ n u m ; 
         i n t   s a r _ d e n ; 
 
         c h a r   * v i d e o _ c o d e c _ i n f o ; 
         c h a r   * a u d i o _ c o d e c _ i n f o ; 
         c h a r   * s u b t i t l e _ c o d e c _ i n f o ; 
         U i n t 3 2   o v e r l a y _ f o r m a t ; 
 
         i n t   l a s t _ e r r o r ; 
         i n t   p r e p a r e d ; 
         i n t   a u t o _ r e s u m e ; 
         i n t   e r r o r ; 
         i n t   e r r o r _ c o u n t ; 
         i n t   s t a r t _ o n _ p r e p a r e d ; 
         i n t   f i r s t _ v i d e o _ f r a m e _ r e n d e r e d ; 
         i n t   f i r s t _ a u d i o _ f r a m e _ r e n d e r e d ; 
         i n t   s y n c _ a v _ s t a r t ; 
 
         M e s s a g e Q u e u e   m s g _ q u e u e ; 
 
         i n t 6 4 _ t   p l a y a b l e _ d u r a t i o n _ m s ; 
 
         i n t   p a c k e t _ b u f f e r i n g ; 
         i n t   p i c t q _ s i z e ; 
         i n t   m a x _ f p s ; 
         i n t   s t a r t u p _ v o l u m e ; 
 
         i n t   v i d e o t o o l b o x ; 
         i n t   v t b _ m a x _ f r a m e _ w i d t h ; 
         i n t   v t b _ a s y n c ; 
         i n t   v t b _ w a i t _ a s y n c ; 
         i n t   v t b _ h a n d l e _ r e s o l u t i o n _ c h a n g e ; 
 
         i n t   m e d i a c o d e c _ a l l _ v i d e o s ; 
         i n t   m e d i a c o d e c _ a v c ; 
         i n t   m e d i a c o d e c _ h e v c ; 
         i n t   m e d i a c o d e c _ m p e g 2 ; 
         i n t   m e d i a c o d e c _ m p e g 4 ; 
         i n t   m e d i a c o d e c _ h a n d l e _ r e s o l u t i o n _ c h a n g e ; 
         i n t   m e d i a c o d e c _ a u t o _ r o t a t e ; 
 
         i n t   o p e n s l e s ; 
         i n t   s o u n d t o u c h _ e n a b l e ; 
 
         c h a r   * i f o r m a t _ n a m e ; 
 
         i n t   n o _ t i m e _ a d j u s t ; 
         d o u b l e   p r e s e t _ 5 _ 1 _ c e n t e r _ m i x _ l e v e l ; 
 
         s t r u c t   I j k M e d i a M e t a   * m e t a ; 
 
         S D L _ S p e e d S a m p l e r   v f p s _ s a m p l e r ; 
         S D L _ S p e e d S a m p l e r   v d p s _ s a m p l e r ; 
 
         / *   f i l t e r s   * / 
         S D L _ m u t e x     * v f _ m u t e x ; 
         S D L _ m u t e x     * a f _ m u t e x ; 
         i n t                   v f _ c h a n g e d ; 
         i n t                   a f _ c h a n g e d ; 
         f l o a t               p f _ p l a y b a c k _ r a t e ; 
         i n t                   p f _ p l a y b a c k _ r a t e _ c h a n g e d ; 
         f l o a t               p f _ p l a y b a c k _ v o l u m e ; 
         i n t                   p f _ p l a y b a c k _ v o l u m e _ c h a n g e d ; 
 
         v o i d                               * i n j e c t _ o p a q u e ; 
         v o i d                               * i j k i o _ i n j e c t _ o p a q u e ; 
         F F S t a t i s t i c                   s t a t ; 
         F F D e m u x C a c h e C o n t r o l   d c c ; 
 
         A V A p p l i c a t i o n C o n t e x t   * a p p _ c t x ; 
         I j k I O M a n a g e r C o n t e x t   * i j k i o _ m a n a g e r _ c t x ; 
 
         i n t   e n a b l e _ a c c u r a t e _ s e e k ; 
         i n t   a c c u r a t e _ s e e k _ t i m e o u t ; 
         i n t   m e d i a c o d e c _ s y n c ; 
         i n t   s k i p _ c a l c _ f r a m e _ r a t e ; 
         i n t   g e t _ f r a m e _ m o d e ; 
         G e t I m g I n f o   * g e t _ i m g _ i n f o ; 
         
 	 
         A V F o r m a t C o n t e x t   * m _ o f m t _ c t x ;                 
         A V O u t p u t F o r m a t   * m _ o f m t ; 
         p t h r e a d _ m u t e x _ t   r e c o r d _ m u t e x ;               
         e n u m   A V R e c o r d S t a t e   i s _ r e c o r d ;                                           
         i n t   r e c o r d _ e r r o r ; 
 / / a d d e d   b y   y u a n z c 
 	 A V P a c k e t   * m _ p a c k e t ; 
 	 i n t   s w i t c h _ a u d i o _ d b ; 
 	 e n u m   A V P l a y T y p e   v i d e o _ p l a y _ t y p e ; 
 / / e n d   a d d e d 
 	 / / i n t   g e t _ i d r ;         
         / / i n t   i s _ f i r s t ;                                               / /   i f   t h e   f i r s t 
         i n t 6 4 _ t   v i d e o _ n u m ;                                     / / v i d e o   f r a m e   n u m b e r 
         i n t 6 4 _ t   a u d i o _ n u m ;                                     / /   a u d i o   f r a m e   n u m b e r 
         i n t 6 4 _ t   v i d e o _ d u r a t i o n ; 
 	 i n t 6 4 _ t   a u d i o _ d u r a t i o n ; 
 	 i n t 6 4 _ t   v i d e o _ s t a r t _ p t s ; 
 	 i n t 6 4 _ t   a u d i o _ s t a r t _ p t s ; 
 	 
         i n t   a s y n c _ i n i t _ d e c o d e r ; 
         c h a r   * v i d e o _ m i m e _ t y p e ; 
         c h a r   * m e d i a c o d e c _ d e f a u l t _ n a m e ; 
         i n t   i j k m e t a _ d e l a y _ i n i t ; 
         i n t   r e n d e r _ w a i t _ s t a r t ; 
 }   F F P l a y e r ; 
 
 # d e f i n e   f f t i m e _ t o _ m i l l i s e c o n d s ( t s )   ( a v _ r e s c a l e ( t s ,   1 0 0 0 ,   A V _ T I M E _ B A S E ) ) 
 # d e f i n e   m i l l i s e c o n d s _ t o _ f f t i m e ( m s )   ( a v _ r e s c a l e ( m s ,   A V _ T I M E _ B A S E ,   1 0 0 0 ) ) 
 
 i n l i n e   s t a t i c   v o i d   f f p _ r e s e t _ i n t e r n a l ( F F P l a y e r   * f f p ) 
 { 
         / *   f f p - > i s   c l o s e d   i n   s t r e a m _ c l o s e ( )   * / 
         a v _ o p t _ f r e e ( f f p ) ; 
 
         / *   f o r m a t / c o d e c   o p t i o n s   * / 
         a v _ d i c t _ f r e e ( & f f p - > f o r m a t _ o p t s ) ; 
         a v _ d i c t _ f r e e ( & f f p - > c o d e c _ o p t s ) ; 
         a v _ d i c t _ f r e e ( & f f p - > s w s _ d i c t ) ; 
         a v _ d i c t _ f r e e ( & f f p - > p l a y e r _ o p t s ) ; 
         a v _ d i c t _ f r e e ( & f f p - > s w r _ o p t s ) ; 
         a v _ d i c t _ f r e e ( & f f p - > s w r _ p r e s e t _ o p t s ) ; 
 
         / *   f f p l a y   o p t i o n s   s p e c i f i e d   b y   t h e   u s e r   * / 
         a v _ f r e e p ( & f f p - > i n p u t _ f i l e n a m e ) ; 
         f f p - > a u d i o _ d i s a b l e                     =   0 ; 
         f f p - > v i d e o _ d i s a b l e                     =   0 ; 
         m e m s e t ( f f p - > w a n t e d _ s t r e a m _ s p e c ,   0 ,   s i z e o f ( f f p - > w a n t e d _ s t r e a m _ s p e c ) ) ; 
         f f p - > s e e k _ b y _ b y t e s                     =   - 1 ; 
         f f p - > d i s p l a y _ d i s a b l e                 =   0 ; 
         f f p - > s h o w _ s t a t u s                         =   0 ; 
         f f p - > a v _ s y n c _ t y p e                       =   A V _ S Y N C _ A U D I O _ M A S T E R ; 
         f f p - > s t a r t _ t i m e                           =   A V _ N O P T S _ V A L U E ; 
         f f p - > d u r a t i o n                               =   A V _ N O P T S _ V A L U E ; 
         f f p - > f a s t                                       =   1 ; 
         f f p - > g e n p t s                                   =   0 ; 
         f f p - > l o w r e s                                   =   0 ; 
         f f p - > d e c o d e r _ r e o r d e r _ p t s         =   - 1 ; 
         f f p - > a u t o e x i t                               =   0 ; 
         f f p - > l o o p                                       =   1 ; 
         f f p - > f r a m e d r o p                             =   0 ;   / /   o p t i o n 
         f f p - > s e e k _ a t _ s t a r t                     =   0 ; 
         f f p - > i n f i n i t e _ b u f f e r                 =   - 1 ; 
         f f p - > s h o w _ m o d e                             =   S H O W _ M O D E _ N O N E ; 
         a v _ f r e e p ( & f f p - > a u d i o _ c o d e c _ n a m e ) ; 
         a v _ f r e e p ( & f f p - > v i d e o _ c o d e c _ n a m e ) ; 
         f f p - > r d f t s p e e d                             =   0 . 0 2 ; 
 # i f   C O N F I G _ A V F I L T E R 
         a v _ f r e e p ( & f f p - > v f i l t e r s _ l i s t ) ; 
         f f p - > n b _ v f i l t e r s                         =   0 ; 
         f f p - > a f i l t e r s                               =   N U L L ; 
         f f p - > v f i l t e r 0                               =   N U L L ; 
 # e n d i f 
         f f p - > a u t o r o t a t e                           =   1 ; 
         f f p - > f i n d _ s t r e a m _ i n f o               =   1 ; 
 
         f f p - > s w s _ f l a g s                             =   S W S _ F A S T _ B I L I N E A R ; 
 
         / *   c u r r e n t   c o n t e x t   * / 
         f f p - > a u d i o _ c a l l b a c k _ t i m e         =   0 ; 
 
         / *   e x t r a   f i e l d s   * / 
         f f p - > a o u t                                       =   N U L L ;   / *   r e s e t   o u t s i d e   * / 
         f f p - > v o u t                                       =   N U L L ;   / *   r e s e t   o u t s i d e   * / 
         f f p - > p i p e l i n e                               =   N U L L ; 
         f f p - > n o d e _ v d e c                             =   N U L L ; 
         f f p - > s a r _ n u m                                 =   0 ; 
         f f p - > s a r _ d e n                                 =   0 ; 
 
         a v _ f r e e p ( & f f p - > v i d e o _ c o d e c _ i n f o ) ; 
         a v _ f r e e p ( & f f p - > a u d i o _ c o d e c _ i n f o ) ; 
         a v _ f r e e p ( & f f p - > s u b t i t l e _ c o d e c _ i n f o ) ; 
         f f p - > o v e r l a y _ f o r m a t                   =   S D L _ F C C _ R V 3 2 ; 
 
         f f p - > l a s t _ e r r o r                           =   0 ; 
         f f p - > p r e p a r e d                               =   0 ; 
         f f p - > a u t o _ r e s u m e                         =   0 ; 
         f f p - > e r r o r                                     =   0 ; 
         f f p - > e r r o r _ c o u n t                         =   0 ; 
         f f p - > s t a r t _ o n _ p r e p a r e d             =   1 ; 
         f f p - > f i r s t _ v i d e o _ f r a m e _ r e n d e r e d   =   0 ; 
         f f p - > s y n c _ a v _ s t a r t                     =   1 ; 
         f f p - > e n a b l e _ a c c u r a t e _ s e e k       =   0 ; 
         f f p - > a c c u r a t e _ s e e k _ t i m e o u t     =   M A X _ A C C U R A T E _ S E E K _ T I M E O U T ; 
 
         f f p - > p l a y a b l e _ d u r a t i o n _ m s                       =   0 ; 
 
         f f p - > p a c k e t _ b u f f e r i n g                               =   1 ; 
         f f p - > p i c t q _ s i z e                                           =   V I D E O _ P I C T U R E _ Q U E U E _ S I Z E _ D E F A U L T ;   / /   o p t i o n 
         f f p - > m a x _ f p s                                                 =   3 1 ;   / /   o p t i o n 
 
         f f p - > v i d e o t o o l b o x                                       =   0 ;   / /   o p t i o n 
         f f p - > v t b _ m a x _ f r a m e _ w i d t h                         =   0 ;   / /   o p t i o n 
         f f p - > v t b _ a s y n c                                             =   0 ;   / /   o p t i o n 
         f f p - > v t b _ h a n d l e _ r e s o l u t i o n _ c h a n g e       =   0 ;   / /   o p t i o n 
         f f p - > v t b _ w a i t _ a s y n c                                   =   0 ;   / /   o p t i o n 
 
         f f p - > m e d i a c o d e c _ a l l _ v i d e o s                     =   0 ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ a v c                                   =   0 ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ h e v c                                 =   0 ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ m p e g 2                               =   0 ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ h a n d l e _ r e s o l u t i o n _ c h a n g e   =   0 ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ a u t o _ r o t a t e                   =   0 ;   / /   o p t i o n 
 
         f f p - > o p e n s l e s                                               =   0 ;   / /   o p t i o n 
         f f p - > s o u n d t o u c h _ e n a b l e                             =   0 ;   / /   o p t i o n 
 
         f f p - > i f o r m a t _ n a m e                                       =   N U L L ;   / /   o p t i o n 
 
         f f p - > n o _ t i m e _ a d j u s t                                   =   0 ;   / /   o p t i o n 
         f f p - > a s y n c _ i n i t _ d e c o d e r                           =   0 ;   / /   o p t i o n 
         f f p - > v i d e o _ m i m e _ t y p e                                 =   N U L L ;   / /   o p t i o n 
         f f p - > m e d i a c o d e c _ d e f a u l t _ n a m e                 =   N U L L ;   / /   o p t i o n 
         f f p - > i j k m e t a _ d e l a y _ i n i t                           =   0 ;   / /   o p t i o n 
         f f p - > r e n d e r _ w a i t _ s t a r t                             =   0 ; 
 
         i j k m e t a _ r e s e t ( f f p - > m e t a ) ; 
 
         S D L _ S p e e d S a m p l e r R e s e t ( & f f p - > v f p s _ s a m p l e r ) ; 
         S D L _ S p e e d S a m p l e r R e s e t ( & f f p - > v d p s _ s a m p l e r ) ; 
 
         / *   f i l t e r s   * / 
         f f p - > v f _ c h a n g e d                                           =   0 ; 
         f f p - > a f _ c h a n g e d                                           =   0 ; 
         f f p - > p f _ p l a y b a c k _ r a t e                               =   1 . 0 f ; 
         f f p - > p f _ p l a y b a c k _ r a t e _ c h a n g e d               =   0 ; 
         f f p - > p f _ p l a y b a c k _ v o l u m e                           =   1 . 0 f ; 
         f f p - > p f _ p l a y b a c k _ v o l u m e _ c h a n g e d           =   0 ; 
 
 	 f f p - > v i d e o _ p l a y _ t y p e   =   A V P L A Y _ L I V E _ N O R M A L ; / /   o p t i o n   a d d e d   b y   y u a n z c 
 	 f f p - > s w i t c h _ a u d i o _ d b   =   0 ; 
 
         a v _ a p p l i c a t i o n _ c l o s e p ( & f f p - > a p p _ c t x ) ; 
         i j k i o _ m a n a g e r _ d e s t r o y p ( & f f p - > i j k i o _ m a n a g e r _ c t x ) ; 
 
         m s g _ q u e u e _ f l u s h ( & f f p - > m s g _ q u e u e ) ; 
 
         f f p - > i n j e c t _ o p a q u e   =   N U L L ; 
         f f p - > i j k i o _ i n j e c t _ o p a q u e   =   N U L L ; 
         f f p _ r e s e t _ s t a t i s t i c ( & f f p - > s t a t ) ; 
         f f p _ r e s e t _ d e m u x _ c a c h e _ c o n t r o l ( & f f p - > d c c ) ; 
 } 
 
 i n l i n e   s t a t i c   v o i d   f f p _ n o t i f y _ m s g 1 ( F F P l a y e r   * f f p ,   i n t   w h a t )   { 
         m s g _ q u e u e _ p u t _ s i m p l e 3 ( & f f p - > m s g _ q u e u e ,   w h a t ,   0 ,   0 ) ; 
 } 
 
 i n l i n e   s t a t i c   v o i d   f f p _ n o t i f y _ m s g 2 ( F F P l a y e r   * f f p ,   i n t   w h a t ,   i n t   a r g 1 )   { 
         m s g _ q u e u e _ p u t _ s i m p l e 3 ( & f f p - > m s g _ q u e u e ,   w h a t ,   a r g 1 ,   0 ) ; 
 } 
 
 i n l i n e   s t a t i c   v o i d   f f p _ n o t i f y _ m s g 3 ( F F P l a y e r   * f f p ,   i n t   w h a t ,   i n t   a r g 1 ,   i n t   a r g 2 )   { 
         m s g _ q u e u e _ p u t _ s i m p l e 3 ( & f f p - > m s g _ q u e u e ,   w h a t ,   a r g 1 ,   a r g 2 ) ; 
 } 
 
 i n l i n e   s t a t i c   v o i d   f f p _ n o t i f y _ m s g 4 ( F F P l a y e r   * f f p ,   i n t   w h a t ,   i n t   a r g 1 ,   i n t   a r g 2 ,   v o i d   * o b j ,   i n t   o b j _ l e n )   { 
         m s g _ q u e u e _ p u t _ s i m p l e 4 ( & f f p - > m s g _ q u e u e ,   w h a t ,   a r g 1 ,   a r g 2 ,   o b j ,   o b j _ l e n ) ; 
 } 
 
 i n l i n e   s t a t i c   v o i d   f f p _ r e m o v e _ m s g ( F F P l a y e r   * f f p ,   i n t   w h a t )   { 
         m s g _ q u e u e _ r e m o v e ( & f f p - > m s g _ q u e u e ,   w h a t ) ; 
 } 
 
 i n l i n e   s t a t i c   c o n s t   c h a r   * f f p _ g e t _ e r r o r _ s t r i n g ( i n t   e r r o r )   { 
         s w i t c h   ( e r r o r )   { 
                 c a s e   A V E R R O R ( E N O M E M ) :               r e t u r n   " A V E R R O R ( E N O M E M ) " ;               / /   1 2 
                 c a s e   A V E R R O R ( E I N V A L ) :               r e t u r n   " A V E R R O R ( E I N V A L ) " ;               / /   2 2 
                 c a s e   A V E R R O R ( E A G A I N ) :               r e t u r n   " A V E R R O R ( E A G A I N ) " ;               / /   3 5 
                 c a s e   A V E R R O R ( E T I M E D O U T ) :         r e t u r n   " A V E R R O R ( E T I M E D O U T ) " ;         / /   6 0 
                 c a s e   A V E R R O R _ E O F :                       r e t u r n   " A V E R R O R _ E O F " ; 
                 c a s e   A V E R R O R _ E X I T :                     r e t u r n   " A V E R R O R _ E X I T " ; 
         } 
         r e t u r n   " u n k n o w n " ; 
 } 
 
 # d e f i n e   F F T R A C E   A L O G W 
 
 # d e f i n e   A V C O D E C _ M O D U L E _ N A M E         " a v c o d e c " 
 # d e f i n e   M E D I A C O D E C _ M O D U L E _ N A M E   " M e d i a C o d e c " 
 
 # e n d i f 
 