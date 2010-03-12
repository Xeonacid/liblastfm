/*
   Copyright 2009 Last.fm Ltd. 
      - Primarily authored by Max Howell, Jono Cole and Doug Mansell

   This file is part of liblastfm.

   liblastfm is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   liblastfm is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with liblastfm.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef LASTFM_USER_H
#define LASTFM_USER_H

#include <lastfm/ws.h>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <lastfm/UserList>

namespace lastfm
{
    class LASTFM_DLLEXPORT User
    {
        QString m_name;
    
    public:
        User() : m_name( lastfm::ws::Username ), m_match( -1.0f )
        {}

        User( const QString& name ) : m_name( name ), m_match( -1.0f )
        {}

        bool operator==(const lastfm::User& that) const { return m_name == that.m_name; }

        operator QString() const { return m_name; }
        QString name() const { return m_name; }
        
    
        /** use Tag::list() on the response to get a WeightedStringList */
        QNetworkReply* getTopTags() const;

        /** use User::list() on the response to get a QList<User> */
        QNetworkReply* getFriends(int perPage = 50, int page = 1) const;
        QNetworkReply* getNeighbours() const;
    
        QNetworkReply* getPlaylists() const;
        QNetworkReply* getTopArtists() const;
        QNetworkReply* getRecentTracks() const;
        QNetworkReply* getRecentArtists() const;
        QNetworkReply* getRecentStations() const;
    
        static UserList list( QNetworkReply* );
    
    //////
        QUrl smallImageUrl() const { return m_smallImage; }
        QUrl mediumImageUrl() const { return m_mediumImage; }
        QUrl largeImageUrl() const { return m_largeImage; }
    
        QString realName() const { return m_realName; }
    
        /** the user's profile page at www.last.fm */
        QUrl www() const;
    
        /** Returns the match between the logged in user and the user which this
          * object represents (if < 0.0f then not set) */
        float match() const { return m_match; }
    
    private:
        QUrl m_smallImage;
        QUrl m_mediumImage;
        QUrl m_largeImage;
    
        float m_match;
    
        QString m_realName;
        
        QMap<QString, QString> params( const QString& method ) const;
    };


    /** The Extended User contains extra information about a user's account */
    class LASTFM_DLLEXPORT UserDetails : public User
    {
    public:
        /** User details */
        UserDetails( QNetworkReply* );
    
        /** you can only get information about the any user */
        static QNetworkReply* getInfo( const QString& username = lastfm::ws::Username );

        /** a verbose string, eg. "A man with 36,153 scrobbles" */
        QString getInfoString();

        bool isSubscriber() const{ return m_isSubscriber; }
        bool canBootstrap() const{ return m_canBootstrap; }

        // pass the result to Artist::list(), if you want the other data 
        // you have to parse the lfm() yourself members
        // http://www.last.fm/api/show?service=388
        // static QNetworkReply* getRecommendedArtists();

    protected:
            
        class Gender
        {
            QString s;

        public:
            Gender() :s(/*confused!*/){}

            Gender( const QString& ss ) :s( ss.toLower() )
            {}
     
            bool known() const { return male() || female(); }
            bool male() const { return s == "m"; }
            bool female() const { return s == "f"; }
     
            QString toString() const
            {
                #define tr QObject::tr
                QStringList list;
                if (male())
                    list << tr("boy") << tr("lad") << tr("chap") << tr("guy");
                else if (female())
                    // I'm not sexist, it's just I'm gutless and couldn't think
                    // of any other non offensive terms for women!
                    list << tr("girl") << tr("lady") << tr("lass");
                else 
                    return tr("person");
                
                return list.value( QDateTime::currentDateTime().toTime_t() % list.count() );
                #undef tr
            }
        } m_gender;

        unsigned short m_age;
        unsigned int m_scrobbles;
        QDateTime m_registered;
        QString m_country;
        bool m_isSubscriber;
        bool m_canBootstrap;
    };
}

#endif
